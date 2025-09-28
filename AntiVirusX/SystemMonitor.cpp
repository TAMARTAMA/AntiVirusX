#include "SystemMonitor.h"
////מנהל את התהליכים פה!
//void SystemMonitor::Start(HINSTANCE hInstance, int nCmdShow) {
//	thread monitorThread(&SystemMonitor::B, this, hInstance, nCmdShow);
//	monitorThread.detach(); // ← Detach הראשון תקין
//
//	thread monitorThreadfile(&SystemMonitor::A, this);
//	monitorThreadfile.detach();
//}
//
//
//void SystemMonitor::A() {
//	wstring foundFile = fileMonitor.StartMonitoring();
//	//if (foundFile != NULL)
//	ScanFile(foundFile);
//}
////TODO בFOR ScanFile להוסיף 
//void SystemMonitor::B(HINSTANCE hInstance, int nCmdShow) 
//{
//	driveMonitor.StartMonitoring(hInstance, nCmdShow); 
//}
//void  SystemMonitor::ScanFile(wstring foundFile) {
//	MiddlewareCheckingFile checkingFile(foundFile);
//
//}