#define WIN32_LEAN_AND_MEAN 

#include "../inc.hpp"
#include <filesystem>
#include <vector>
#include "../mapper/include/kdmapper.hpp"
#include "../mapper/include/intel_driver.hpp"
#include "../mapper/include/utils.hpp"

bool IsDriverLoaded() {
    HANDLE hDevice = CreateFileW(L"\\\\.\\{77777777-7777-7777-7777-777777777777}", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(hDevice);
        return true;
    }
    // std::cout << "[-] IsDriverLoaded failed with error: " << GetLastError() << std::endl;
    return false;
}

void ShutdownDriver() {
    HANDLE hDevice = CreateFileW(L"\\\\.\\{77777777-7777-7777-7777-777777777777}", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice != INVALID_HANDLE_VALUE) {
        DWORD bytes;
        if (DeviceIoControl(hDevice, soardrv_shutdown, NULL, 0, NULL, 0, &bytes, NULL)) {
            std::cout << "[+] Driver shutdown command sent." << std::endl;
        } else {
            std::cout << "[-] Failed to send shutdown command." << std::endl;
        }
        CloseHandle(hDevice);
    }
}

int main( ) {
	SetConsoleTitle( L"dev by Ali" );

    if (IsDriverLoaded()) {
        std::cout << "[+] Driver is already loaded. Skipping mapping." << std::endl;
        goto waitForGame;
    }

    // KDMAPPER INTEGRATION
    {
        const std::wstring driver_path = L"driver.sys";
        if (!std::filesystem::exists(driver_path)) {
            std::cout << "[-] driver.sys not found" << std::endl;
            system("pause");
            return -1;
        }

        if (!NT_SUCCESS(intel_driver::Load())) {
            std::cout << "[-] Failed to load vulnerable driver" << std::endl;
            system("pause");
            return -1;
        }

        std::vector<uint8_t> raw_image = { 0 };
        if (!kdmUtils::ReadFileToMemory(driver_path, &raw_image)) {
            std::cout << "[-] Failed to read driver to memory" << std::endl;
            intel_driver::Unload();
            system("pause");
            return -1;
        }

        NTSTATUS exitCode = 0;
        if (!kdmapper::MapDriver(reinterpret_cast<BYTE*>(raw_image.data()), 0, 0, false, true, kdmapper::AllocationMode::AllocateIndependentPages, false, nullptr, &exitCode)) {
            std::cout << "[-] Failed to map driver" << std::endl;
            intel_driver::Unload();
            system("pause");
            return -1;
        }

        intel_driver::Unload();
        std::cout << "[+] Driver mapped successfully!" << std::endl;
    }

waitForGame:
    std::cout << "[+] waiting for game..." << std::endl;

    while (true) {
        auto process_status = _proc_manager.attach("cs2.exe");
        if (process_status == SUCCEED) {
            std::cout << "[+] success" << std::endl;
            // FreeConsole(); // Removed to keep console visible
            break;
        }
        
        // Debug output
        if (process_status == FAILE_PROCESSID) {
            // Don't spam if just waiting for game
            // std::cout << "[-] Waiting for game (Process not found)..." << std::endl;
        } else {
            std::cout << "[-] Attach failed with error: " << process_status << std::endl;
            if (process_status == FAILE_HPROCESS) {
                 std::cout << "[-] Failed to open driver handle. Is the driver loaded?" << std::endl;
            } else if (process_status == FAILE_MODULE) {
                 std::cout << "[-] Failed to get module base. Driver communication error." << std::endl;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

	if ( !unknowncheats::framework::m_b_initialized )
		unknowncheats::framework::create( );

	while ( !unknowncheats::_interfaces->initialize( ) ) {
		printf( "[unknowncheats] failed to init offsets, retrying...\n" );
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
	}

	printf( "[unknowncheats] initialized interfaces!\n" );

	if ( !unknowncheats::_address->initialize( ) ) {
		printf( "[unknowncheats] failtd to init addresses\n" );
		goto END;
	}

	while ( !unknowncheats::framework::unloading ) {
		if ( !unknowncheats::framework::render( ) )
			return 0;

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

END:
    ShutdownDriver();
	std::cout << std::endl;
    std::cout << "[+] Cheat finished. Console will stay open." << std::endl;
	while(true) { // Keep console open
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
	return 0;
}