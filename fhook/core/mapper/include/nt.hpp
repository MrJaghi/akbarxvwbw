#pragma once

// این پرگماها باید همیشه در ابتدای فایل باشند تا روی تمام تعاریف تأثیر بگذارند.
#pragma pack(push, 8)

// 1. Windows.h را برای توابع استاندارد Win32 فراخوانی می‌کنیم.
#include <Windows.h>

// 2. سپس winternl.h را برای ساختارهای سطح بالاتر NT API فراخوانی می‌کنیم.
#include <winternl.h>

// ntdef.h معمولاً توسط Windows.h یا winternl.h به درستی مدیریت می‌شود.
// حذف آن از اینجا تداخلات را حل می‌کند.
// #include <ntdef.h>
// -------------------------

// پروژه را به کتابخانه ntdll.lib لینک می‌کنیم تا توابع NT API پیدا شوند.
#pragma comment(lib, "ntdll.lib")

// به طور موقت هشدار C4005 (تعریف مجدد ماکرو) را غیرفعال می‌کنیم.
#pragma warning(push)
#pragma warning(disable: 4005)
#include <ntstatus.h>
#pragma warning(pop)

// تمام تعاریف را در یک namespace به نام 'nt' قرار می‌دهیم تا از تداخل نام جلوگیری شود.
namespace nt
{
    // مقادیر مربوط به کلاس اطلاعات سیستم برای تابع NtQuerySystemInformation
    constexpr auto SystemModuleInformation = 11;
    constexpr auto SystemExtendedHandleInformation = 64;

    // تعریف توابع نیتیو NT API
    extern "C" NTSTATUS NTAPI NtLoadDriver(PUNICODE_STRING DriverServiceName);
    extern "C" NTSTATUS NTAPI NtUnloadDriver(PUNICODE_STRING DriverServiceName);
    extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN Client, PBOOLEAN WasEnabled);

    // ساختارهای زیر به صورت دستی و مطابق با ساختارهای کرنل ویندوز تعریف شده‌اند.
    // این تعاریف باید با نسخه ویندوزی که هدف قرار داده‌اید، مطابقت داشته باشند.

    typedef struct _SYSTEM_HANDLE
    {
        PVOID Object;
        HANDLE UniqueProcessId;
        HANDLE HandleValue;
        ULONG GrantedAccess;
        USHORT CreatorBackTraceIndex;
        USHORT ObjectTypeIndex;
        ULONG HandleAttributes;
        ULONG Reserved;
    } SYSTEM_HANDLE, *PSYSTEM_HANDLE;

    typedef struct _SYSTEM_HANDLE_INFORMATION_EX
    {
        ULONG_PTR HandleCount;
        ULONG_PTR Reserved;
        SYSTEM_HANDLE Handles[1];
    } SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

    typedef enum class _POOL_TYPE {
        NonPagedPool,
        NonPagedPoolExecute = NonPagedPool,
        PagedPool,
        NonPagedPoolMustSucceed = NonPagedPool + 2,
        DontUseThisType,
        NonPagedPoolCacheAligned = NonPagedPool + 4,
        PagedPoolCacheAligned,
        NonPagedPoolCacheAlignedMustS = NonPagedPool + 6,
        MaxPoolType,
        NonPagedPoolBase = 0,
        NonPagedPoolBaseMustSucceed = NonPagedPoolBase + 2,
        NonPagedPoolBaseCacheAligned = NonPagedPoolBase + 4,
        NonPagedPoolBaseCacheAlignedMustS = NonPagedPoolBase + 6,
        NonPagedPoolSession = 32,
        PagedPoolSession = NonPagedPoolSession + 1,
        NonPagedPoolMustSucceedSession = PagedPoolSession + 1,
        DontUseThisTypeSession = NonPagedPoolMustSucceedSession + 1,
        NonPagedPoolCacheAlignedSession = DontUseThisTypeSession + 1,
        PagedPoolCacheAlignedSession = NonPagedPoolCacheAlignedSession + 1,
        NonPagedPoolCacheAlignedMustSSession = PagedPoolCacheAlignedSession + 1,
        NonPagedPoolNx = 512,
        NonPagedPoolNxCacheAligned = NonPagedPoolNx + 4,
        NonPagedPoolSessionNx = NonPagedPoolNx + 32,
    } POOL_TYPE;

    typedef struct _RTL_PROCESS_MODULE_INFORMATION
    {
        HANDLE Section;
        PVOID MappedBase;
        PVOID ImageBase;
        ULONG ImageSize;
        ULONG Flags;
        USHORT LoadOrderIndex;
        USHORT InitOrderIndex;
        USHORT LoadCount;
        USHORT OffsetToFileName;
        UCHAR FullPathName[256];
    } RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

    typedef struct _RTL_PROCESS_MODULES
    {
        ULONG NumberOfModules;
        RTL_PROCESS_MODULE_INFORMATION Modules[1];
    } RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

    typedef struct _HashBucketEntry
    {
        struct _HashBucketEntry* Next;
        UNICODE_STRING DriverName;
        ULONG CertHash[5];
    } HashBucketEntry, * PHashBucketEntry;


    typedef struct _RTL_BALANCED_LINKS {
        struct _RTL_BALANCED_LINKS* Parent;
        struct _RTL_BALANCED_LINKS* LeftChild;
        struct _RTL_BALANCED_LINKS* RightChild;
        CHAR Balance;
        UCHAR Reserved[3];
    } RTL_BALANCED_LINKS;
    typedef RTL_BALANCED_LINKS* PRTL_BALANCED_LINKS;

    typedef struct _RTL_AVL_TABLE {
        RTL_BALANCED_LINKS BalancedRoot;
        PVOID OrderedPointer;
        ULONG WhichOrderedElement;
        ULONG NumberGenericTableElements;
        ULONG DepthOfTree;
        PVOID RestartKey;
        ULONG DeleteCount;
        PVOID CompareRoutine;
        PVOID AllocateRoutine;
        PVOID FreeRoutine;
        PVOID TableContext;
    } RTL_AVL_TABLE;
    typedef RTL_AVL_TABLE* PRTL_AVL_TABLE;

    typedef struct _PiDDBCacheEntry
    {
        LIST_ENTRY		List;
        UNICODE_STRING	DriverName;
        ULONG			TimeDateStamp;
        NTSTATUS		LoadStatus;
        char			_0x0028[16];
    } PiDDBCacheEntry, * NPiDDBCacheEntry;
}

// تنظیمات چینش حافظه را به حالت اولیه بازمی‌گردانیم.
#pragma pack(pop)