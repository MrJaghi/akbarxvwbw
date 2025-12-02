#pragma once
#include "definitions.h"
#include <stdint.h>

// Obfuscated IOCTL codes - change these values before each compile
// Use random values to avoid pattern detection
#define soardrv_read_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A7F, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define soardrv_write_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3B8E, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define soardrv_get_process_base CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4C9D, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) 
#define soardrv_get_cr3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x5DAC, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define soardrv_shutdown CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6EBD, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Global variable (initialized in main.c)
extern uintptr_t cr3_base;

// Structure definitions with proper alignment
#pragma pack(push, 8)

typedef struct _soardrv_cr3 {
	ULONG pid;
	ULONG64 base;
	ULONG64 cr3;
} soardrv_cr3, *soardrv_cr3_mod;

typedef struct _soardrv_base {
	ULONG pid;
	ULONG64 handle;
} soardrv_base, *soardrv_base_mod;

typedef struct _soardrv_rw {
	ULONG pid;
	ULONG64 src;
	ULONG64 dst;
	ULONG64 size;
} soardrv_rw, *soardrv_rw_mod;

#pragma pack(pop)

// Validation macros
#define VALIDATE_POINTER(ptr) ((ptr) != NULL && MmIsAddressValid(ptr))
#define VALIDATE_BUFFER_SIZE(size) ((size) > 0 && (size) <= 0x100000) // Max 1MB
#define VALIDATE_ADDRESS_RANGE(addr) ((ULONG64)(addr) >= 0x1000 && (ULONG64)(addr) < 0x7FFFFFFFFFFF)