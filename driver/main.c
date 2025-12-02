#pragma warning(disable: 6320 30029 4047 4024 4133)
#include "definitions.h"
#include "ioctls.h"
#include <ntimage.h>
#include <intrin.h>

// Random device names - change these before each compile
#define drv_device L"\\Device\\{77777777-7777-7777-7777-777777777777}"
#define drv_dos_device L"\\DosDevices\\{77777777-7777-7777-7777-777777777777}"

// XOR key for string obfuscation - change before each compile
#define XOR_KEY 0xAB

PDEVICE_OBJECT driver_object = NULL;
UNICODE_STRING dev, dos;
uintptr_t dirbase = 0;

// Auto-cleanup variables
HANDLE cleanup_thread_handle = NULL;
BOOLEAN should_stop_cleanup = FALSE;
ULONG monitored_cs2_pid = 0;
ULONG monitored_cheat_pid = 0;

// Obfuscated strings (XOR encrypted at compile time)
CHAR g_DeviceName[] = { 0xC3, 0xA4, 0xCE, 0xCB, 0xC9, 0xC7, 0xCE, 0xA4 }; // "\\Device\\"
CHAR g_DosDevices[] = { 0xC3, 0xA4, 0xD1, 0xCE, 0xC8, 0xD1, 0xCE, 0xC7, 0xC9, 0xC7, 0xCE, 0xA4 }; // "\\DosDevices\\"

// XOR string obfuscation
VOID XorDecrypt(PCHAR str, SIZE_T len, UCHAR key)
{
	for (SIZE_T i = 0; i < len; i++)
		str[i] ^= key;
}

// Anti-debugging checks (multi-layer)
BOOLEAN IsDebuggerPresent()
{
	// Check 1: Kernel debugger
	if (KD_DEBUGGER_ENABLED || !KD_DEBUGGER_NOT_PRESENT)
		return TRUE;

	// Check 2: System debug flag
	if (*(PULONG)((PUCHAR)PsInitialSystemProcess + 0x2fc) & 0x400)
		return TRUE;

	// Check 3: Timing attack (debugger slows down execution)
	LARGE_INTEGER StartTime, EndTime;
	KeQuerySystemTime(&StartTime);
	
	// Dummy operations
	volatile int x = 0;
	for (int i = 0; i < 100; i++)
		x += i;
	
	KeQuerySystemTime(&EndTime);
	
	// If took more than 1ms, likely debugged
	if ((EndTime.QuadPart - StartTime.QuadPart) > 10000)
		return TRUE;

	return FALSE;
}

// Anti-obfuscation: Random delay with RDTSC
VOID RandomDelay()
{
	LARGE_INTEGER interval;
	ULONG random = (ULONG)(__rdtsc() % 150);
	interval.QuadPart = -(10000LL * random);
	KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

// Obfuscated function pointers to avoid static analysis
typedef NTSTATUS(*fnRtlGetVersion)(PRTL_OSVERSIONINFOW);
typedef NTSTATUS(*fnMmCopyMemory)(PVOID, MM_COPY_ADDRESS, SIZE_T, ULONG, PSIZE_T);

fnRtlGetVersion pRtlGetVersion = NULL;
fnMmCopyMemory pMmCopyMemory = NULL;

// Resolve function dynamically to avoid IAT hooks
PVOID GetKernelFunction(PUCHAR FunctionName)
{
	UNICODE_STRING name;
	RtlInitAnsiString((PANSI_STRING)&name, (PCSZ)FunctionName);
	
	PVOID func = MmGetSystemRoutineAddress((PUNICODE_STRING)&name);
	return func;
}

// Get CR3 offset (hardened with obfuscation)
// **********************************************************************************
// ***************************** START OF FIXED CODE ********************************
// **********************************************************************************
uintptr_t GetUserDirectoryTableBaseOffset()
{
	// The offset of DirectoryTableBase within the _KPROCESS structure
	// is 0x28 for most modern versions of Windows 10 and 11.
	// The previous hardcoded values were incorrect and caused the failure.
	return 0x28;
}
// **********************************************************************************
// ****************************** END OF FIXED CODE *********************************
// **********************************************************************************

// Read physical memory with MmCopyMemory (safer, less detectable)
NTSTATUS ReadPhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead)
{
	if (!TargetAddress || !lpBuffer || !BytesRead)
		return STATUS_INVALID_PARAMETER;

	if ((ULONG64)TargetAddress < 0x1000)
		return STATUS_ACCESS_VIOLATION;

	*BytesRead = 0;

	if (!pMmCopyMemory)
		pMmCopyMemory = (fnMmCopyMemory)GetKernelFunction((PUCHAR)"MmCopyMemory");

	if (!pMmCopyMemory)
		return STATUS_NOT_FOUND;

	MM_COPY_ADDRESS AddrToRead = { 0 };
	AddrToRead.PhysicalAddress.QuadPart = (LONGLONG)TargetAddress;

	SIZE_T BytesCopied = 0;
	NTSTATUS status = pMmCopyMemory(lpBuffer, AddrToRead, Size, MM_COPY_MEMORY_PHYSICAL, &BytesCopied);

	if (NT_SUCCESS(status))
		*BytesRead = BytesCopied;

	RandomDelay();
	return status;
}

// Write physical memory (hardened)
NTSTATUS WritePhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten)
{
	if (!TargetAddress || !lpBuffer || !BytesWritten)
		return STATUS_INVALID_PARAMETER;

	if ((ULONG64)TargetAddress < 0x1000 || (ULONG64)TargetAddress > 0x7FFFFFFFFFFF)
		return STATUS_ACCESS_VIOLATION;

	*BytesWritten = 0;

	PHYSICAL_ADDRESS AddrToWrite = { 0 };
	AddrToWrite.QuadPart = (LONGLONG)TargetAddress;

	PVOID pmapped_mem = MmMapIoSpace(AddrToWrite, Size, MmNonCached);
	if (!pmapped_mem)
		return STATUS_UNSUCCESSFUL;

	__try
	{
		// Use RtlCopyMemory with SEH
		RtlCopyMemory(pmapped_mem, lpBuffer, Size);
		*BytesWritten = Size;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		MmUnmapIoSpace(pmapped_mem, Size);
		return STATUS_ACCESS_VIOLATION;
	}

	MmUnmapIoSpace(pmapped_mem, Size);
	RandomDelay();
	return STATUS_SUCCESS;
}

// Translate virtual to physical address (page walk)
INT64 TranslateLinearAddress(uintptr_t directoryTableBase, ULONG64 virtualAddress)
{
	if (!directoryTableBase || !virtualAddress)
		return 0;

	// Cast to ULONG64 for pointer arithmetic
	ULONG64 va = virtualAddress;
	
	// Extract page table indices
	ULONG64 pml4_index = (va >> 39) & 0x1FF;
	ULONG64 pdpt_index = (va >> 30) & 0x1FF;
	ULONG64 pd_index = (va >> 21) & 0x1FF;
	ULONG64 pt_index = (va >> 12) & 0x1FF;
	ULONG64 offset = va & 0xFFF;

	// Read PML4E
	ULONG64 pml4e = 0;
	SIZE_T readSize = 0;
	if (!NT_SUCCESS(ReadPhysicalAddress(
		(PVOID)(directoryTableBase + pml4_index * sizeof(ULONG64)),
		&pml4e, sizeof(pml4e), &readSize)) || !pml4e || !(pml4e & 1))
		return 0;

	// Read PDPTE
	ULONG64 pdpte = 0;
	if (!NT_SUCCESS(ReadPhysicalAddress(
		(PVOID)((pml4e & PMASK) + pdpt_index * sizeof(ULONG64)),
		&pdpte, sizeof(pdpte), &readSize)) || !pdpte || !(pdpte & 1))
		return 0;

	// Check for 1GB page
	if (pdpte & 0x80)
		return (pdpte & (~0ull << 30)) + (va & ~(~0ull << 30));

	// Read PDE
	ULONG64 pde = 0;
	if (!NT_SUCCESS(ReadPhysicalAddress(
		(PVOID)((pdpte & PMASK) + pd_index * sizeof(ULONG64)),
		&pde, sizeof(pde), &readSize)) || !pde || !(pde & 1))
		return 0;

	// Check for 2MB page
	if (pde & 0x80)
		return (pde & PMASK) + (va & ~(~0ull << 21));

	// Read PTE
	ULONG64 pte = 0;
	if (!NT_SUCCESS(ReadPhysicalAddress(
		(PVOID)((pde & PMASK) + pt_index * sizeof(ULONG64)),
		&pte, sizeof(pte), &readSize)) || !pte || !(pte & 1))
		return 0;

	// Return physical address
	return (pte & PMASK) + offset;
}

// Check if process exists by PID
BOOLEAN IsProcessAlive(ULONG pid)
{
	if (!pid)
		return FALSE;

	PEPROCESS proc = NULL;
	NTSTATUS status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)pid, &proc);

	if (NT_SUCCESS(status) && proc)
	{
		ObDereferenceObject(proc);
		return TRUE;
	}

	return FALSE;
}

// Cleanup thread that monitors processes
VOID CleanupThreadRoutine(PVOID context)
{
	UNREFERENCED_PARAMETER(context);

	while (!should_stop_cleanup)
	{
		// Sleep for 5 seconds
		LARGE_INTEGER interval;
		interval.QuadPart = -(50000000LL); // 5 seconds
		KeDelayExecutionThread(KernelMode, FALSE, &interval);

		// Check if both processes are dead
		BOOLEAN cs2_alive = IsProcessAlive(monitored_cs2_pid);
		BOOLEAN cheat_alive = IsProcessAlive(monitored_cheat_pid);

		if (!cs2_alive && !cheat_alive && (monitored_cs2_pid != 0 || monitored_cheat_pid != 0))
		{
			// Both processes are dead, unload driver
			should_stop_cleanup = TRUE;

			// Cleanup device
			if (driver_object)
			{
				IoDeleteSymbolicLink(&dos);
				IoDeleteDevice(driver_object);
				driver_object = NULL;
			}

			break;
		}
	}

	PsTerminateSystemThread(STATUS_SUCCESS);
}

// Get process base address
NTSTATUS get_base_address(ULONG pid, PULONG64 base_address)
{
	if (!base_address)
		return STATUS_INVALID_PARAMETER;

	*base_address = 0;

	PEPROCESS target_proc = NULL;
	NTSTATUS status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)pid, &target_proc);

	if (!NT_SUCCESS(status) || !target_proc)
		return STATUS_INVALID_PARAMETER;

	// Get base from EPROCESS
	PVOID base = PsGetProcessSectionBaseAddress(target_proc);
	if (base)
		*base_address = (ULONG64)base;
	else
		status = STATUS_NOT_FOUND;

	ObDereferenceObject(target_proc);
	RandomDelay();
	return status;
}

// Unsupported IRP handler
NTSTATUS unsupported_dispatch(PDEVICE_OBJECT device, PIRP irp)
{
	UNREFERENCED_PARAMETER(device);

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_NOT_SUPPORTED;
}

// Basic IRP handler
NTSTATUS dispatch(PDEVICE_OBJECT device, PIRP irp)
{
	UNREFERENCED_PARAMETER(device);

	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

// Main IOCTL handler (hardened)
NTSTATUS io_device_control(PDEVICE_OBJECT device, PIRP irp)
{
	UNREFERENCED_PARAMETER(device);

	// Anti-debug check on every IOCTL
	if (IsDebuggerPresent())
	{
		irp->IoStatus.Status = STATUS_ACCESS_DENIED;
		irp->IoStatus.Information = 0;
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return STATUS_ACCESS_DENIED;
	}

	NTSTATUS status = STATUS_SUCCESS;
	ULONG_PTR info_size = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);
	ULONG ctl_code = stack->Parameters.DeviceIoControl.IoControlCode;
	ULONG input_len = stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG output_len = stack->Parameters.DeviceIoControl.OutputBufferLength;
	PVOID buffer = irp->AssociatedIrp.SystemBuffer;

	__try
	{
		switch (ctl_code)
		{
		case soardrv_get_cr3:
		{
			if (output_len < sizeof(soardrv_cr3))
			{
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}

			soardrv_cr3_mod in = (soardrv_cr3_mod)buffer;
			if (!in || !in->pid)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			// Store PID for monitoring
			if (monitored_cs2_pid == 0)
				monitored_cs2_pid = in->pid;

			// FIXED: Use the PID from input to get target process CR3
			PEPROCESS target_proc = NULL;
			status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)in->pid, &target_proc);

			if (!NT_SUCCESS(status) || !target_proc)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			ULONG64 process_dirbase = 0;
			uintptr_t offset = GetUserDirectoryTableBaseOffset();

			if (offset)
			{
				// Read CR3 from EPROCESS using byte pointer arithmetic
				PUCHAR proc_ptr = (PUCHAR)target_proc;
				RtlCopyMemory(&process_dirbase, proc_ptr + offset, sizeof(process_dirbase));
				dirbase = process_dirbase;
				in->cr3 = process_dirbase;
				in->base = (ULONG64)target_proc;
			}

			ObDereferenceObject(target_proc);

			if (!dirbase)
			{
				status = STATUS_NOT_FOUND;
				break;
			}

			info_size = sizeof(soardrv_cr3);
			status = STATUS_SUCCESS;
			break;
		}

		case soardrv_read_physical:
		{
			if (input_len < sizeof(soardrv_rw))
			{
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}

			soardrv_rw_mod in = (soardrv_rw_mod)buffer;
			if (!in || !in->src || !in->dst || !in->size || in->size > 0x100000)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			if (!dirbase)
			{
				status = STATUS_DEVICE_NOT_READY;
				break;
			}

			PEPROCESS target_proc = NULL;
			status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)in->pid, &target_proc);
			if (!NT_SUCCESS(status) || !target_proc)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			ObDereferenceObject(target_proc);

			SIZE_T CurOffset = 0;
			SIZE_T TotalSize = in->size;

			while (TotalSize > 0)
			{
				INT64 CurPhysAddr = TranslateLinearAddress(dirbase, in->src + CurOffset);
				if (!CurPhysAddr)
				{
					status = STATUS_UNSUCCESSFUL;
					break;
				}

				ULONG64 ReadSize = min(PAGE_SIZE - (CurPhysAddr & 0xFFF), TotalSize);
				SIZE_T BytesRead = 0;

				status = ReadPhysicalAddress((PVOID)CurPhysAddr, (PVOID)(in->dst + CurOffset), ReadSize, &BytesRead);

				if (!NT_SUCCESS(status) || BytesRead == 0)
					break;

				TotalSize -= BytesRead;
				CurOffset += BytesRead;
			}

			info_size = sizeof(soardrv_rw);
			break;
		}

		case soardrv_write_physical:
		{
			if (input_len < sizeof(soardrv_rw))
			{
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}

			soardrv_rw_mod in = (soardrv_rw_mod)buffer;
			if (!in || !in->src || !in->dst || !in->size || in->size > 0x100000)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			if (!dirbase)
			{
				status = STATUS_DEVICE_NOT_READY;
				break;
			}

			PEPROCESS target_proc = NULL;
			status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)in->pid, &target_proc);
			if (!NT_SUCCESS(status) || !target_proc)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			ObDereferenceObject(target_proc);

			SIZE_T CurOffset = 0;
			SIZE_T TotalSize = in->size;

			while (TotalSize > 0)
			{
				INT64 CurPhysAddr = TranslateLinearAddress(dirbase, in->src + CurOffset);
				if (!CurPhysAddr)
				{
					status = STATUS_UNSUCCESSFUL;
					break;
				}

				ULONG64 WriteSize = min(PAGE_SIZE - (CurPhysAddr & 0xFFF), TotalSize);
				SIZE_T BytesWritten = 0;

				status = WritePhysicalAddress((PVOID)CurPhysAddr, (PVOID)(in->dst + CurOffset), WriteSize, &BytesWritten);

				if (!NT_SUCCESS(status) || BytesWritten == 0)
					break;

				TotalSize -= BytesWritten;
				CurOffset += BytesWritten;
			}

			info_size = sizeof(soardrv_rw);
			break;
		}

		case soardrv_get_process_base:
		{
			if (input_len < sizeof(soardrv_base) || output_len < sizeof(soardrv_base))
			{
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}

			soardrv_base_mod in = (soardrv_base_mod)buffer;
			if (!in || !in->pid)
			{
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			status = get_base_address(in->pid, &in->handle);
			info_size = sizeof(soardrv_base);
			break;
		}

		case soardrv_shutdown:
		{
			if (driver_object)
			{
				IoDeleteSymbolicLink(&dos);
				IoDeleteDevice(driver_object);
				driver_object = NULL;
			}
			status = STATUS_SUCCESS;
			info_size = 0;
			break;
		}

		default:
			status = STATUS_INVALID_DEVICE_REQUEST;
			info_size = 0;
			break;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		status = STATUS_ACCESS_VIOLATION;
		info_size = 0;
	}

	irp->IoStatus.Status = status;
	irp->IoStatus.Information = info_size;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return status;
}

// Unload driver
VOID unload_driver(PDRIVER_OBJECT driver)
{
	UNREFERENCED_PARAMETER(driver);

	if (driver_object)
	{
		IoDeleteSymbolicLink(&dos);
		IoDeleteDevice(driver_object);
		driver_object = NULL;
	}
}

// Initialization (hardened with obfuscation)
NTSTATUS init(PDRIVER_OBJECT driver, PUNICODE_STRING path)
{
	UNREFERENCED_PARAMETER(path);

	// Multi-layer anti-debug
	if (IsDebuggerPresent())
		return STATUS_ACCESS_DENIED;

	RandomDelay();

	NTSTATUS status = STATUS_SUCCESS;

	RtlInitUnicodeString(&dev, drv_device);
	RtlInitUnicodeString(&dos, drv_dos_device);

	// Clean up any existing device
	IoDeleteSymbolicLink(&dos);
	if (driver_object)
		IoDeleteDevice(driver_object);

	status = IoCreateDevice(driver, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &driver_object);
	if (!NT_SUCCESS(status))
		return status;

	status = IoCreateSymbolicLink(&dos, &dev);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(driver_object);
		driver_object = NULL;
		return status;
	}

	// Setup dispatch routines
	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
		driver->MajorFunction[i] = &unsupported_dispatch;

	driver->MajorFunction[IRP_MJ_CREATE] = &dispatch;
	driver->MajorFunction[IRP_MJ_CLOSE] = &dispatch;
	driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &io_device_control;
	driver->DriverUnload = &unload_driver;

	driver_object->Flags |= DO_BUFFERED_IO;
	driver_object->Flags &= ~DO_DEVICE_INITIALIZING;

	// Initialize function pointers dynamically
	pRtlGetVersion = (fnRtlGetVersion)GetKernelFunction((PUCHAR)"RtlGetVersion");
	pMmCopyMemory = (fnMmCopyMemory)GetKernelFunction((PUCHAR)"MmCopyMemory");

	// Create cleanup monitoring thread
	CLIENT_ID clientId;
	OBJECT_ATTRIBUTES objAttr;
	InitializeObjectAttributes(&objAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

	status = PsCreateSystemThread(
		&cleanup_thread_handle,
		THREAD_ALL_ACCESS,
		&objAttr,
		NULL,
		&clientId,
		CleanupThreadRoutine,
		NULL
	);

	if (!NT_SUCCESS(status))
		cleanup_thread_handle = NULL;

	RandomDelay();
	return STATUS_SUCCESS;
}

// Entry point
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING path)
{
	UNREFERENCED_PARAMETER(path);
	UNREFERENCED_PARAMETER(driver);

	return IoCreateDriver(NULL, &init);
}