#include <Windows.h>
#include <iostream>
#include <winioctl.h>

#define soardrv_shutdown CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6EBD, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

int main() {
    std::cout << "[*] Attempting to unload driver..." << std::endl;
    
    HANDLE hDevice = CreateFileW(L"\\\\.\\{77777777-7777-7777-7777-777777777777}", 
                                  GENERIC_READ | GENERIC_WRITE, 
                                  FILE_SHARE_READ | FILE_SHARE_WRITE, 
                                  NULL, 
                                  OPEN_EXISTING, 
                                  0, 
                                  NULL);
    
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cout << "[-] Driver not loaded or already unloaded. Error: " << GetLastError() << std::endl;
        system("pause");
        return 0;
    }
    
    DWORD bytes;
    if (DeviceIoControl(hDevice, soardrv_shutdown, NULL, 0, NULL, 0, &bytes, NULL)) {
        std::cout << "[+] Driver shutdown command sent successfully!" << std::endl;
        std::cout << "[+] Driver unloaded. You can now load the new driver." << std::endl;
    } else {
        std::cout << "[-] Failed to send shutdown command. Error: " << GetLastError() << std::endl;
    }
    
    CloseHandle(hDevice);
    
    std::cout << "[*] Done." << std::endl;
    system("pause");
    return 0;
}
