#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <Tlhelp32.h>
#include <atlconv.h>
#include "driver_interface.hpp"

#define _is_invalid(v,n) if(v==NULL) return n

enum StatusCode {
	SUCCEED,
	FAILE_PROCESSID,
	FAILE_HPROCESS,
	FAILE_MODULE,
};

class ProcessManager {
private:

	bool   Attached = false;

public:

	HANDLE hProcess = 0;
	HANDLE hDriver = INVALID_HANDLE_VALUE;
	DWORD  ProcessID = 0;
	DWORD64  ModuleAddress = 0;

public:
	~ProcessManager( ) {
	}

	StatusCode attach( std::string ProcessName ) {
		ProcessID = this->get_process_id( ProcessName );
		_is_invalid( ProcessID, FAILE_PROCESSID );

		hDriver = CreateFileA("\\\\.\\{77777777-7777-7777-7777-777777777777}", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (hDriver == INVALID_HANDLE_VALUE) return FAILE_HPROCESS;

		// Open handle for VirtualQueryEx (Pattern Scanning)
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, TRUE, ProcessID );

		printf("[DEBUG] sizeof(_soardrv_cr3) = %zu bytes\n", sizeof(_soardrv_cr3));
		printf("[DEBUG] ProcessID = %d\n", ProcessID);

		_soardrv_base base_req;
		base_req.pid = ProcessID;
		base_req.handle = 0;
		DWORD bytes;
		if (!DeviceIoControl(hDriver, soardrv_get_process_base, &base_req, sizeof(base_req), &base_req, sizeof(base_req), &bytes, 0))
			return FAILE_MODULE;

		ModuleAddress = base_req.handle;
		_is_invalid( ModuleAddress, FAILE_MODULE );

		printf("[DEBUG] ModuleAddress = 0x%llX\n", ModuleAddress);

		// Initialize driver CR3
		_soardrv_cr3 cr3_req;
		cr3_req.pid = ProcessID;
		cr3_req.base = ModuleAddress;
		cr3_req.cr3 = 0;
		
		BOOL ioctl_result = DeviceIoControl(hDriver, soardrv_get_cr3, &cr3_req, sizeof(cr3_req), &cr3_req, sizeof(cr3_req), &bytes, 0);
		
		printf("[DEBUG] DeviceIoControl result: %d, CR3: 0x%llX, Bytes: %d\n", ioctl_result, cr3_req.cr3, bytes);
		
		if (!ioctl_result) {
			printf("[process_manager] Failed to get CR3 from driver. Error: %d\n", GetLastError());
			return FAILE_MODULE;
		}
		
		printf("[process_manager] CR3 initialized: 0x%llX for PID: %d\n", cr3_req.cr3, ProcessID);

		Attached = true;

		return SUCCEED;
	}

	void detach( ) {
		if ( hDriver != INVALID_HANDLE_VALUE )
			CloseHandle( hDriver );
		hDriver = INVALID_HANDLE_VALUE;
		if ( hProcess )
			CloseHandle( hProcess );
		hProcess = 0;
		ProcessID = 0;
		ModuleAddress = 0;
		Attached = false;
	}

	bool is_active( ) {
		if ( !Attached )
			return false;
		DWORD ExitCode{};
		// Since we don't have a process handle, we can check if we can still read memory or get base address
		// For simplicity, we'll assume it's active if we can read the base address signature or similar
		// Or just return true if Attached is true, but that's not robust.
		// Let's try to get the base address again as a check
		_soardrv_base base_req;
		base_req.pid = ProcessID;
		uintptr_t check_addr = 0;
		base_req.handle = (uintptr_t)&check_addr;
		DWORD bytes;
		if (!DeviceIoControl(hDriver, soardrv_get_process_base, &base_req, sizeof(base_req), &base_req, sizeof(base_req), &bytes, 0))
			return false;
		return check_addr != 0;
	}

	template <typename ReadType>
	bool read_memory( DWORD64 Address, ReadType& Value, int Size ) {
		_is_invalid( hDriver, false );
		_is_invalid( ProcessID, false );

		_soardrv_rw req;
		req.pid = ProcessID;
		req.src = Address;
		req.dst = (ULONGLONG)&Value;
		req.size = Size;

		DWORD bytes;
		if (DeviceIoControl(hDriver, soardrv_read_physical, &req, sizeof(req), &req, sizeof(req), &bytes, 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool read_memory( DWORD64 Address, ReadType& Value ) {
		_is_invalid( hDriver, false );
		_is_invalid( ProcessID, false );

		_soardrv_rw req;
		req.pid = ProcessID;
		req.src = Address;
		req.dst = (ULONGLONG)&Value;
		req.size = sizeof( ReadType );

		DWORD bytes;
		if (DeviceIoControl(hDriver, soardrv_read_physical, &req, sizeof(req), &req, sizeof(req), &bytes, 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool write_memory( DWORD64 Address, ReadType& Value, int Size ) {
		_is_invalid( hDriver, false );
		_is_invalid( ProcessID, false );

		_soardrv_rw req;
		req.pid = ProcessID;
		req.src = Address;
		req.dst = (ULONGLONG)&Value;
		req.size = Size;

		DWORD bytes;
		if (DeviceIoControl(hDriver, soardrv_write_physical, &req, sizeof(req), &req, sizeof(req), &bytes, 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool write_memory( DWORD64 Address, ReadType& Value ) {
		_is_invalid( hDriver, false );
		_is_invalid( ProcessID, false );

		_soardrv_rw req;
		req.pid = ProcessID;
		req.src = Address;
		req.dst = (ULONGLONG)&Value;
		req.size = sizeof( ReadType );

		DWORD bytes;
		if (DeviceIoControl(hDriver, soardrv_write_physical, &req, sizeof(req), &req, sizeof(req), &bytes, 0))
			return true;
		return false;
	}

	template <typename ReadType>
	bool write_memory_nuke( DWORD64 Address, ReadType& Value ) {
		_is_invalid( hDriver, false );
		_is_invalid( ProcessID, false );

		_soardrv_rw req;
		req.pid = ProcessID;
		req.src = Address;
		req.dst = (ULONGLONG)&Value;
		req.size = sizeof( ReadType );

		DWORD bytes;
		if (DeviceIoControl(hDriver, soardrv_write_physical, &req, sizeof(req), &req, sizeof(req), &bytes, 0))
			return true;
		//return false;

		return true;
	}

	std::vector<DWORD64> search_memory( const std::string& Signature, DWORD64 StartAddress, DWORD64 EndAddress, int SearchNum = 1 );

	DWORD64 trace_address( DWORD64 BaseAddress, std::vector<DWORD> Offsets ) {
		_is_invalid( hDriver, 0 );
		_is_invalid( ProcessID, 0 );
		DWORD64 Address = 0;

		if ( Offsets.size( ) == 0 )
			return BaseAddress;

		if ( !read_memory<DWORD64>( BaseAddress, Address ) )
			return 0;

		for ( size_t i = 0; i < Offsets.size( ) - 1; i++ ) {
			if ( !read_memory<DWORD64>( Address + Offsets[ i ], Address ) )
				return 0;
		}
		return Address == 0 ? 0 : Address + Offsets[ Offsets.size( ) - 1 ];
	}

public:

	DWORD get_process_id( std::string ProcessName ) {
		PROCESSENTRY32 ProcessInfoPE;
		ProcessInfoPE.dwSize = sizeof( PROCESSENTRY32 );
		HANDLE hSnapshot = CreateToolhelp32Snapshot( 15, 0 );
		Process32First( hSnapshot, &ProcessInfoPE );
		USES_CONVERSION;
		do {
			if ( strcmp( W2A( ProcessInfoPE.szExeFile ), ProcessName.c_str( ) ) == 0 ) {
				CloseHandle( hSnapshot );
				return ProcessInfoPE.th32ProcessID;
			}
		} while ( Process32Next( hSnapshot, &ProcessInfoPE ) );
		CloseHandle( hSnapshot );
		return 0;
	}

	HMODULE get_process_module_handle( std::string ModuleName ) {
		MODULEENTRY32 ModuleInfoPE;
		ModuleInfoPE.dwSize = sizeof( MODULEENTRY32 );
		HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, this->ProcessID );
		Module32First( hSnapshot, &ModuleInfoPE );
		USES_CONVERSION;
		do {
			if ( strcmp( W2A( ModuleInfoPE.szModule ), ModuleName.c_str( ) ) == 0 ) {
				CloseHandle( hSnapshot );
				return ModuleInfoPE.hModule;
			}
		} while ( Module32Next( hSnapshot, &ModuleInfoPE ) );
		CloseHandle( hSnapshot );
		return 0;
	}

};

inline ProcessManager _proc_manager;