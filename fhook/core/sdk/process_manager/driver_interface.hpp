#pragma once
#include <Windows.h>
#include <winioctl.h>
#include <cstdint>

#define soardrv_read_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A7F, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define soardrv_write_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3B8E, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define soardrv_get_process_base CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4C9D, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) 
#define soardrv_get_cr3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5DAC, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define soardrv_shutdown CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6EBD, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#pragma pack(push, 8)

typedef struct soardrv_cr3 {
	ULONG pid;
	std::uintptr_t base;
	std::uintptr_t cr3;
} _soardrv_cr3, * soardrv_cr3_mod;

typedef struct soardrv_base {
	ULONG pid;
	std::uintptr_t handle;
} _soardrv_base, * soardrv_base_mod;

typedef struct soardrv_rw {
	ULONG pid;
	ULONGLONG src;
	ULONGLONG dst;
	ULONGLONG size;
} _soardrv_rw, * soardrv_rw_mod;

#pragma pack(pop)
