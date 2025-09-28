#pragma once
#ifndef DRIVE_MONITOR_h
#define DRIVE_MONITOR_h
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte
#include <iostream>
#include <string>
#include <Dbt.h>
#include <filesystem>
#include "Global.h"
#include <functional>
#include <thread>
namespace fs = std::filesystem;
class DriveMonitor {
public:
    void StartMonitoring(HINSTANCE hInstance, int nCmdShow,std::function<void(std::vector<FileExe> files)> );

private:
  static  std::function<void(std::vector<FileExe> files)> callbackToScanfiles;
    static std::vector<FileExe> files; 
    static HWND hListBox; 
    static void ScanDriveForExeFiles(const std::string& drivePath) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(drivePath)) {
                files.push_back(entry.path().wstring());
            }
        }
        catch (const std::exception& e) {
           std:: wcout << L"Error scanning drive: "  << L" - " << e.what() << std::endl;
        }
        std::thread scanFiles(CallScanFiles);
        scanFiles.detach();
    }
    static void CallScanFiles() {
    callbackToScanfiles(files);
    }
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
        case WM_DEVICECHANGE:
            if (wParam == DBT_DEVICEARRIVAL) 
            {
                auto* pHdr = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
                if (pHdr && pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    auto* pVol = reinterpret_cast<PDEV_BROADCAST_VOLUME>(lParam);
                    if (pVol)
                    {
                        wchar_t driveLetter = L'A';
                        DWORD mask = pVol->dbcv_unitmask;
                        while (!(mask & 1)) {
                            mask >>= 1;
                            driveLetter++;
                        }
                        std::string drivePath = std::string(1, driveLetter) + ":\\";
                        //Global::callbackToShowMessenge("New drive detected: " + drivePath);
                        ScanDriveForExeFiles(drivePath);
                    }
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    void MonitorDrives(HINSTANCE hInstance, int nCmdShow);
};
#endif //DRIVE_MONITOR_h