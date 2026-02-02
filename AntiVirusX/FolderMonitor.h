

#pragma once
#ifndef FOLDER_MONITOR_H
#define FOLDER_MONITOR_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte   
#include <shlobj.h>         
#include <cstddef>          
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <functional>
#include "Global.h"
using std::wstring;
using std::cout;
using std::endl;

namespace fs = std::filesystem;
class FolderMonitor {
private:
	std::function<void(FileExe fileExe)> callbackToScan;
	HANDLE hDir;
	std::wstring monitorPath;
	OVERLAPPED overlapped;
	char buffer[4096];
	DWORD bytesReturned;

public:


	~FolderMonitor();
	void StartMonitoring(std::function<void(FileExe fileExe)> );
	void stop();
	void Start();
	bool getDownloadsPath();


	bool MonitorFolder();

	bool isTemporaryFile(const std::wstring& filename);


	void processNotifications(DWORD bytesTransferred);
};
#endif //FOLDER_MONITOR_H
