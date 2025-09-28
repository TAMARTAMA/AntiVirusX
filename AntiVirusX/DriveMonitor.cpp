#include "DriveMonitor.h"
std::vector<FileExe> DriveMonitor::files;
 std::function<void(std::vector<FileExe> files)> DriveMonitor::callbackToScanfiles=nullptr;
void DriveMonitor::StartMonitoring(HINSTANCE hInstance, int nCmdShow, std::function<void(std::vector<FileExe> files)> callbackToScanfiles)
{
	this->callbackToScanfiles = callbackToScanfiles;
		MonitorDrives(hInstance, nCmdShow);
}

void DriveMonitor::MonitorDrives(HINSTANCE hInstance, int nCmdShow) {

	const wchar_t CLASS_NAME[] = L"DriveMonitorWindowClass";
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&wc)) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to register window class!");
		return;
	}

	HWND hWnd = CreateWindowEx(
		0,                 
		CLASS_NAME,        
		L"Drive Monitor",  
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,     
		CW_USEDEFAULT,     
		500,               
		300,               
		NULL,              
		NULL,              
		hInstance,         
		NULL               
	);

	if (!hWnd) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to create window!");
		return ;
	}

	ShowWindow(hWnd, 0);
	UpdateWindow(hWnd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)&&Global::running) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}