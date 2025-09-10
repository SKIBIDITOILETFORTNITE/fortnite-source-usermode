#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <memory>

#define RDWCode CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9400, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define SHACode CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9401, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define FGACode CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9401, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CR3Code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9403, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define SecurityCode 0x9ab7f61
uintptr_t Base;
#define MOUSE_LEFT_BUTTON_DOWN   0x0001  // Left Button changed to down.
#define MOUSE_LEFT_BUTTON_UP     0x0002  // Left Button changed to up.
// read/write operations
typedef struct _RD {
    INT32 security;
    INT32 process_id;
    ULONGLONG address;
    ULONGLONG buffer;
    ULONGLONG size;
    BOOLEAN write;
} RD, * RDW;

// structs for driver communication
typedef struct _SH
{
    INT32 security;
    INT32 process_id;
    ULONGLONG* address;
} SH, * SHA;

typedef struct _FA
{
    INT32 security;
    ULONGLONG* address;
} FA, * FGA;

typedef struct _MEMORY_OPERATION_DATA
{
    uint32_t pid;
    ULONGLONG* cr3;
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;

namespace driver
{
    HANDLE driver_handle;
    INT32 process_id;

    uintptr_t base_address;
    uintptr_t cr3;

    bool FindDriverHandle()
    {
        driver_handle = CreateFileW((L"\\\\.\\{SmartSGN}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

        if (driver_handle == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Failed to open driver handle." << std::endl;
            return false;
        }
        return true;
    }

    void ReadPhysical(PVOID address, PVOID buffer, DWORD size)
    {
        _RD arguments = { SecurityCode, process_id, (ULONGLONG)address, (ULONGLONG)buffer, size, FALSE };

        DWORD bytesReturned;
        DeviceIoControl(driver_handle, RDWCode, &arguments, sizeof(arguments), nullptr, NULL, &bytesReturned, NULL);
    }

    void WritePhysical(PVOID address, PVOID buffer, DWORD size)
    {
        _RD arguments = { SecurityCode, process_id, (ULONGLONG)address, (ULONGLONG)buffer, size, TRUE };

        DWORD bytesReturned;
        DeviceIoControl(driver_handle, RDWCode, &arguments, sizeof(arguments), nullptr, NULL, &bytesReturned, NULL);
    }

    uintptr_t DecryptCr3()
    {
        uintptr_t cr3 = NULL;
        MEMORY_OPERATION_DATA arguments = { process_id, &cr3 };

        DWORD bytesReturned;
        if (!DeviceIoControl(driver_handle, CR3Code, &arguments, sizeof(arguments), nullptr, NULL, &bytesReturned, NULL)) {
            std::cerr << "Failed To Get Cr3" << std::endl;
        }

        return cr3;
    }

    uintptr_t FindImage()
    {
        uintptr_t image_address = NULL;
        _SH arguments = { SecurityCode, process_id, &image_address };

        DWORD bytesReturned;
        if (!DeviceIoControl(driver_handle, SHACode, &arguments, sizeof(arguments), nullptr, NULL, &bytesReturned, NULL)) {
            std::cerr << "FindImage Failed" << std::endl;
        }

        return image_address;
    }

    INT32 FindProcess(LPCTSTR process_name)
    {
        PROCESSENTRY32 pt;
        HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hsnap, &pt)) {
            do {
                if (!lstrcmpi(pt.szExeFile, process_name)) {
                    CloseHandle(hsnap);
                    process_id = pt.th32ProcessID;
                    return pt.th32ProcessID;
                }
            } while (Process32Next(hsnap, &pt));
        }
        CloseHandle(hsnap);
        return NULL;
    }
}

class driver_memory
{
public:
    bool IsValidAddress(const uint64_t address)
    {
        return address != 0 && address != 0xCCCCCCCCCCCCCCCC && address != 0xFFFFFFFFFFFFFFFF && address > 0x400000 && address <= 0x7FFFFFFFFFFFFFFF;
    }

    template <typename T>
    T Read(uint64_t address)
    {
        T buffer{};
        if (IsValidAddress(address))
        {
            driver::ReadPhysical(reinterpret_cast<PVOID>(address), &buffer, sizeof(T));
        }
        return buffer;
    }

    template <typename T>
    void Write(uint64_t address, T buffer)
    {
        if (IsValidAddress(address)) {
            driver::WritePhysical(reinterpret_cast<PVOID>(address), &buffer, sizeof(T));
        }
    }

    inline bool Read_Array(const std::uintptr_t address, void* buffer, const std::size_t size)
    {
        if (buffer == nullptr || size == 0)
        {
            return false;
        }
        driver::ReadPhysical(reinterpret_cast<PVOID>(address), buffer, static_cast<DWORD>(size));
    }
}; inline driver_memory memory;