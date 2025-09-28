#include "Global.h"

Error_* Global::objLog = nullptr;
bool Global::running = true;
std::function<void(std::string)> Global::callbackToShowMessenge;

std::wstring base = L"C:\\Users\\USER\\Desktop\\tichnut\\B\\Poject\\AntiVirus\\AntiVirusX";
std::string baseS = "C:\\Users\\USER\\Desktop\\tichnut\\B\\Poject\\AntiVirus\\AntiVirusX";

std::wstring Global::fileLogSourceVM = L"C:\\Users\\User\\Desktop\\Monitor\\detailed_trace_log.txt";
std::wstring Global::fileLogAntiVirusX = base+L"\\logAntiVirusX.txt";
std::wstring Global::fileLogDesHost = base + L"\\logETW.txt";
std::wstring Global::filePathDestionVM = L"C:\\Users\\User\\Desktop\\Monitor\\file.exe";
std::wstring Global::vmName = L"USER1";
//std::wstring Global::filePathCredVM = base + L"\\cred.xml";
std::wstring Global::filePathCredVM = base + L"\\credtinal.xml";
std::wstring Global::checkPoint = L"PreRunCheckpoint";
std::string Global::filePathYaraRulesDynamic = baseS + "\\yaraRulesDynamic.txt";
std::string Global::filePathYaraRulesStatic = baseS + "\\yaraRulesStatic.txt";
std::wstring Global::fileETWMonitor = L"C:\\Users\\USER\\Desktop\\Monitor\\TryETWCsharp.exe";

std::string Global::Failed = "Failed ";
std::string Global::Success = "Success ";
std::string Global::Error_a = "Error ";
std::string Global::Virus = "Virus ";
std::string Global::Start = "Start ";
std::string Global::Anlysis = "Anlysis ";
std::string Global::File = "File : ";
std::string Global::AntiVirusX = "AntiVirusX ";
std::string Global::Info = "Info ";
std::string Global::Scanning = "Scanning ";
std::string Global::wstringToStringG(const std::wstring& wstr) {
	if (wstr.empty()) return {};

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size_needed - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
	return result;
}
Global::Global(std::function<void(std::string)> callbackToShowMessenge) {
	Global::objLog = new Error_(this->fileLogAntiVirusX);
	Global::running = true;
	Global::callbackToShowMessenge = callbackToShowMessenge;
}

void Global::SetRunning()
{
	Global::running = !Global::running;
}
Global::~Global()
{
	delete objLog;
}
