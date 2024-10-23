#include <cassert>
#include <iostream>
#include <vector>
#include <windows.h>
#include "dpDefs.h"
#include "dpRCodes.h"
#include "dpMatch.h"
#include "DPDevClt.h"
#include "DpUIApi.h"
#include "dpFtrEx.h"

void printGUIDs(const std::vector<GUID> &devicesGUIDS) {
    try {
        for (int i{0}; i < devicesGUIDS.size(); i++) {
            if (wchar_t guidString[40]; StringFromGUID2(devicesGUIDS.at(i), guidString, 40)) {
                std::cout << "Device " << i + 1 << ": ";
                std::wcout << guidString << std::endl;
            } else {
                std::cerr << "Failed to convert GUID to string" << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "PrintGUIDs Error: " << e.what() << std::endl;
    }
}

std::vector<GUID> enumerateDevices() {
    GUID *deviceUID = nullptr;
    ULONG deviceCount = 0;

    if (const HRESULT enumerationResult = DPFPEnumerateDevices(&deviceCount, &deviceUID); FAILED(enumerationResult)) {
        std::cerr << "Failed to enumerate devices. Error: " << enumerationResult << std::endl;
        throw std::runtime_error("Failed to enumerate devices");
    }
    if (deviceCount == 0) {
        std::cout << "No DP device found." << std::endl;
        char input;
        while (true) {
            std::cout << "Retry search for devices? (y/n): ";
            std::cin >> input;
            switch (input) {
                case 'y':
                    return enumerateDevices();
                case 'n':
                    std::cout << "Exiting." << std::endl;
                    throw std::runtime_error("No device(s) found.");
                default:
                    std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
            }
        }
    }
    std::cout << "Device(s) found: " << deviceCount << std::endl;
    std::vector devices(deviceUID, deviceUID + deviceCount);


    if (deviceUID == nullptr) {
        std::cerr << "Error: deviceUID is nullptr" << std::endl;
        return {};
    }

    return devices;
}

void printDPDeviceInfo(const GUID &device) {
    DP_DEVICE_INFO deviceInfo;
    const HRESULT getDeviceInfoResult = DPFPGetDeviceInfo(device, &deviceInfo);

    if (getDeviceInfoResult != S_OK) {
        std::cout << "Failed to get device info. Error: " << getDeviceInfoResult << std::endl;
    }

    std::wcout << L"Device serial number: " << deviceInfo.HwInfo.szSerialNb << std::endl;
    std::wcout << L"Device product: " << deviceInfo.HwInfo.szProduct << std::endl;
    std::wcout << L"Device vendor: " << deviceInfo.HwInfo.szVendor << std::endl;
    std::wcout << L"Device language: " << deviceInfo.HwInfo.uLanguageId << std::endl;
    DP_DEVICE_VERSION firmwareVersion = deviceInfo.HwInfo.FirmwareRevision;
    DP_DEVICE_VERSION hardwareVersion = deviceInfo.HwInfo.HardwareRevision;
    std::wcout << "Device firmware version (Major.Minor.Build): " << firmwareVersion.uMajor << "." << firmwareVersion.uMinor << "." <<
            firmwareVersion.uBuild << std::endl;
    std::wcout << "Device hardware version (Major.Minor.Build): " << hardwareVersion.uMajor << "." << hardwareVersion.uMinor << "." <<
            hardwareVersion.uBuild << std::endl;
}


int main() {
    std::cout << "Initializing DPFP SDK..." << std::endl;
    assert(DPFPInit() == S_OK);
    std::cout << "DPFPInit returned S_OK" << std::endl;
    std::cout << std::endl;

    std::cout << "Enumerating devices..." << std::endl;
    const std::vector<GUID> foundDevices = enumerateDevices();
    std::cout << std::endl;

    std::cout << "Printing found devices UIDs (serial numbers)..." << std::endl;
    printGUIDs(foundDevices);
    std::cout << std::endl;

    std::cout << "First device found will be used..." << std::endl;
    GUID currentDevice = foundDevices.at(0);
    std::cout << std::endl;

    std::cout << "Printing device info..." << std::endl;
    printDPDeviceInfo(currentDevice);
    std::cout << std::endl;

    std::cout << "Terminating DPFP SDK..." << std::endl;
    DPFPTerm();

    std::cout << "Program end." << std::endl;
    return 0;
}
