#include "ThreatMonitoring.h"


void  ScanFile(FileExe fileExe) {
	Global::objLog->WriteToLog(LogLevel::INFO, "scan file : " + fileExe.wstringToStringF(fileExe.GetPathFile()));
	MiddlewareCheckingFile checkingFile(fileExe);
}

void  ThreatMonitoring::ScanFilePath(std::string fileExe) {
	std::wstring s = std::wstring(fileExe.begin(), fileExe.end());
	ScanFile(FileExe(s));
}

void  ScanFiles(std::vector<FileExe> files) {
	auto i = files.begin();
	for (auto& file : files) {
		file.SetStateFile(StateFile::MONITORING);
		//Sleep(10000);
		ScanFile(file);
	}
}

void callbackFromMonitor(FileExe fileExe)
{
	fileExe.SetStateFile(StateFile::MONITORING);
	ScanFile(fileExe);
}
void callbackFilesFromMonitor(std::vector<FileExe> files)
{
	ScanFiles(files);
}
void  ThreatMonitoring::Start(HINSTANCE hInstance, int nCmdShow) {
	std::thread monitorThread(&ThreatMonitoring::B, this, hInstance, nCmdShow);
	monitorThread.detach(); 

	std::thread monitorThreadfile(&ThreatMonitoring::A, this);
	monitorThreadfile.detach();
	Global::objLog->WriteToLog(LogLevel::INFO, "הסריקה החלה ");
}
void ThreatMonitoring::A() {
	fileMonitor.StartMonitoring(callbackFromMonitor);
}
void ThreatMonitoring::B(HINSTANCE hInstance, int nCmdShow)
{
	driveMonitor.StartMonitoring(hInstance, nCmdShow, callbackFilesFromMonitor);
}


ThreatMonitoring::~ThreatMonitoring()
{
	if (DynamicAnlysisFile::VM != nullptr) {
		delete DynamicAnlysisFile::VM;
		DynamicAnlysisFile::VM = nullptr;
	}
}


