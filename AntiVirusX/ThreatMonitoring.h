

#pragma once
#ifndef THREAD_MONORTING_H
#define THREAD_MONORTING_H
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include "DriveMonitor.h"
#include "MiddlewareCheckingFile.h"
#include "DynamicAnlysisFile.h"
#include "Global.h"
#include "FolderMonitor.h"
using std::wstring;

class ThreatMonitoring {
private:
	FolderMonitor fileMonitor;
	DriveMonitor driveMonitor;
	bool running=false;
	void A();
	void B(HINSTANCE hInstance, int nCmdShow);
	
public:
	void Start(HINSTANCE , int );
	~ThreatMonitoring();
	static void  ScanFilePath(std::string fileExe);

};
#endif //THREAD_MONORTING_H


