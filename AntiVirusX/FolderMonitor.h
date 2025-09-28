
//#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#undef byte
//#include <Dbt.h>
//#include <iostream>
//#include <string>
//#include <thread>
//
//#include "global.h"
//#include "FileExe.h"
//#pragma comment(lib, "setupapi.lib")
//#pragma comment(lib, "user32.lib")
//#define WM_FILE_DOWNLOADED (WM_APP + 1)
//using namespace std;
//class FolderMonitor {
//	public:
//		FileExe StartMonitoring(std::wstring );
//	
//	private:
//		//void ScanFile(const std::wstring& filePath);
//	
//		FileExe MonitorFolder(const std::wstring& );
//
//		bool IsFileUnlocked(const std::wstring& filePath);
//
//	
//		//void ShowMessage(const std::wstring& message);
//};


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