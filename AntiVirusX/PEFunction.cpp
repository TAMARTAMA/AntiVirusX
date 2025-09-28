#include "PEFunction.h"

const std::unordered_set<std::string> WIN_API = {
	"CreateWindowEx", "DefWindowProc", "DispatchMessage", "GetMessage", "PostQuitMessage",
	"RegisterClassEx", "TranslateMessage", "BeginPaint", "EndPaint", "GetClientRect",
	"LoadCursor", "LoadIcon", "PostMessage", "SendMessage", "SetWindowText",
	"GetWindowText", "DrawText", "UpdateWindow", "CreateBrushIndirect", "CreateFontIndirect",
	"CreatePen", "DeleteObject", "GetStockObject", "SelectObject", "BitBlt",
	"CreateCompatibleBitmap", "CreateCompatibleDC", "DeleteDC", "GetDeviceCaps", "SetStretchBltMode",
	"StretchBlt", "TextOut", "GetTextExtentPoint32", "Ellipse", "Polygon",
	"Rectangle", "RoundRect", "CreateFile", "ReadFile", "WriteFile",
	"CloseHandle", "SetFilePointer", "FlushFileBuffers", "GetFileSize", "GetFileAttributes",
	"SetFileAttributes", "DeleteFile", "MoveFile", "CopyFile", "CreateDirectory",
	"RemoveDirectory", "GetDiskFreeSpace", "GetVolumeInformation", "FindFirstFile", "FindNextFile",
	"FindClose", "GetCurrentDirectory", "SetCurrentDirectory", "GetTempPath", "GetTempFileName",
	"SetEndOfFile", "DeviceIoControl", "GlobalAlloc", "GlobalFree", "GlobalLock",
	"GlobalUnlock", "GlobalSize", "LocalAlloc", "LocalFree", "LocalLock",
	"LocalUnlock", "LocalSize", "VirtualAlloc", "VirtualFree", "VirtualQuery",
	"VirtualProtect", "HeapCreate", "HeapDestroy", "HeapAlloc", "HeapFree",
	"HeapSize", "HeapLock", "HeapUnlock", "HeapCompact", "HeapValidate",
	"HeapWalk", "GetProcessHeap", "GetProcAddress", "LoadLibrary", "FreeLibrary",
	"ExitProcess", "GetModuleHandle", "WaitForSingleObject", "CreateThread", "CreateProcess",
	"GetCurrentProcess", "GetCurrentThread", "GetExitCodeProcess", "GetExitCodeThread", "GetStartupInfo",
	"OpenProcess", "TerminateProcess", "TerminateThread", "SuspendThread", "ResumeThread",
	"CloseHandle", "SetHandleInformation", "DuplicateHandle", "GetStdHandle", "SetStdHandle",
	"IsDebuggerPresent", "DebugBreak", "OutputDebugString", "ContinueDebugEvent", "WaitForDebugEvent",
	"SetThreadContext", "GetThreadContext", "ReadProcessMemory", "WriteProcessMemory", "CreateRemoteThread",
	"DebugActiveProcess", "GetLastError", "SetLastError", "FormatMessage", "LocalFree",
	"Sleep", "GetTickCount", "QueryPerformanceCounter", "QueryPerformanceFrequency", "GetCurrentThreadId",
	"GetCurrentProcessId", "SetUnhandledExceptionFilter", "IsProcessorFeaturePresent", "GetSystemTime", "SetSystemTime",
	"GetLocalTime", "SetLocalTime", "GetSystemTimeAdjustment", "GetSystemVersion", "GetSystemDirectory",
	"GetWindowsDirectory", "GetDriveType", "GetSystemInfo", "SystemParametersInfo", "GetUserName",
	"GetComputerName", "GetVersionEx", "OutputDebugString", "GetPrivateProfileString", "WritePrivateProfileString",
	"GetPrivateProfileInt", "WritePrivateProfileSection", "GetProfileString", "WriteProfileString", "GetProfileInt"
};

const std::unordered_set<std::string> FUNCTIONS_LIST = {
	"CreateFileW", "CloseHandle", "ReadFile", "WriteFile", "SetFilePointer",
	"GetFileSize", "DeleteFileW", "MoveFileW", "CopyFileW", "GetFileAttributesW",
	"SetFileAttributesW", "LoadLibraryW", "GetProcAddress", "FreeLibrary", "OpenProcess",
	"GetLastError", "SetLastError", "GlobalAlloc", "GlobalFree", "VirtualAlloc",
	"VirtualFree", "HeapAlloc", "HeapFree", "InitializeCriticalSection", "DeleteCriticalSection",
	"EnterCriticalSection", "LeaveCriticalSection", "CreateMutexW", "ReleaseMutex", "WaitForSingleObject",
	"CreateEventW", "SetEvent", "ResetEvent", "GetModuleHandleW", "GetModuleFileNameW",
	"TerminateProcess", "ExitProcess", "GetProcessHeap", "HeapSize", "HeapReAlloc",
	"LoadLibraryExW", "IsDebuggerPresent", "QueryPerformanceCounter", "GetCurrentProcess", "GetSystemTimeAsFileTime",
	"GetTickCount", "Sleep", "MultiByteToWideChar", "WideCharToMultiByte", "RegOpenKeyExW",
	"RegCloseKey", "RegQueryValueExW", "RegSetValueExW", "RegDeleteValueW", "RegCreateKeyExW",
	"FindResourceW", "LoadResource", "SizeofResource", "CreateThread", "TerminateThread",
	"GetExitCodeThread", "GetThreadContext", "SetThreadContext", "SuspendThread", "ResumeThread",
	"CreateWindowExW", "ShowWindow", "UpdateWindow", "DefWindowProcW", "GetMessageW",
	"TranslateMessage", "DispatchMessageW", "PostQuitMessage", "PostMessageW", "SendMessageW",
	"SetWindowTextW", "GetWindowTextW", "LoadCursorW", "SetCursor", "InvalidateRect",
	"BeginPaint", "EndPaint", "GetDC", "ReleaseDC", "GetClientRect",
	"DestroyWindow", "RegisterClassExW", "LoadIconW", "LoadMenuW", "CreatePopupMenu",
	"AppendMenuW", "TrackPopupMenu", "GetCursorPos", "SetMenu", "GetSubMenu",
	"ShellExecuteW", "DialogBoxParamW", "EndDialog", "GetDlgItem", "SetDlgItemTextW",
	"GetDlgItemTextW", "MessageBeep", "MessageBoxW", "OpenClipboard", "CloseClipboard",
	"EmptyClipboard", "SetClipboardData", "GetClipboardData", "RegisterClipboardFormatW", "IsClipboardFormatAvailable",
	"FindWindowW", "GetWindowRect", "MoveWindow", "SetTimer", "KillTimer",
	"GetSystemMetrics", "ClientToScreen", "ScreenToClient", "GetDesktopWindow", "SetForegroundWindow",
	"GetForegroundWindow", "IsWindowVisible", "ShowCursor", "SetCapture", "ReleaseCapture",
	"MapVirtualKeyW", "GetKeyState", "GetAsyncKeyState", "SendMessageTimeoutW", "DrawTextW",
	"BeginPath", "EndPath", "StrokeAndFillPath", "TextOutW", "ExtTextOutW",
	"CreatePen", "DeleteObject", "SelectObject", "MoveToEx", "LineTo",
	"Rectangle", "Ellipse", "CreateSolidBrush", "CreateFontIndirectW", "SetTextColor",
	"SetBkColor", "SetBkMode", "GetStockObject", "CreateCompatibleDC", "DeleteDC",
	"BitBlt", "StretchBlt", "SaveDC", "RestoreDC", "CreateCompatibleBitmap",
	"GetObjectW", "GetCurrentObject", "SetPixel", "GetPixel", "CreateBitmap",
	"PlaySoundW", "WaveOutGetNumDevs", "WaveOutGetDevCapsW", "WaveOutOpen", "WaveOutClose",
	"WaveOutPrepareHeader", "WaveOutUnprepareHeader", "WaveOutWrite", "WaveOutGetPosition", "WaveOutSetVolume",
	"CoInitialize", "CoUninitialize", "CoCreateInstance", "CoTaskMemAlloc", "CoTaskMemFree"
};
const std::unordered_set<std::string> CMD_COMMAND_LIST = {
	"assoc", "attrib", "bcdedit", "cd", "chkdsk", "cls", "copy", "del", "dir", "diskpart",
	"driverquery", "echo", "exit", "find", "findstr", "format", "help", "ipconfig", "md", "more",
	"move", "net", "netstat", "nslookup", "path", "ping", "powercfg", "rd", "reg", "regedit",
	"rename", "rmdir", "robocopy", "route", "sc", "sfc", "shutdown", "sort", "start", "systeminfo",
	"taskkill", "tasklist", "time", "timeout", "title", "tree", "type", "ver", "vol", "xcopy"
};

std::vector<std::string> getStrings(const std::wstring& filePath) {
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "cant open" + Global::wstringToStringG(filePath) + "file to binary");
		return {};
	}
	auto isMeaningful = [](const std::string& str) -> bool {
		if (str.length() < 4) return false;
		if (std::all_of(str.begin(), str.end(), isdigit)) return false; // רק מספרים
		if (str.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos)
			return false; 
		return true;
		};
	std::vector<std::string> result;
	std::string current_string;
	const size_t bufferSize = 8192;
	char buffer[bufferSize];

	while (file.read(buffer, bufferSize) || file.gcount()) {
		size_t readBytes = file.gcount();
		for (size_t i = 0; i < readBytes; ++i) {
			char ch = buffer[i];
			if (ch >= 32 && ch <= 126) {
				current_string += ch;
			}
			else {
				if (isMeaningful(current_string)) {
					result.push_back(current_string);
				}
				current_string.clear();
			}
		}
	}
	if (isMeaningful(current_string) ) {
		result.push_back(current_string);
	}
	return result;
}
void PEFunction::SetCount(int count)
{
	 this->count=count;
}
int PEFunction::GetCount()
{
	return count;
}

std::shared_ptr<PEFunction> ScanBinaryFile(const std::wstring& filePath)
{
	const std::vector<std::string> excartStrings = getStrings(filePath);
	
	auto p = std::make_shared<PEFunction>();
	p->countAllStrings = excartStrings.size();

	std::regex url_pattern(R"((http:\/\/|https:\/\/|ftp:\/\/|ftps:\/\/|www\.)[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(/\S*)?$)");
	std::regex ip_pattern(R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))");
	std::regex path_pattern(R"([^\\/:*?"<>|\r\n]+(?:\\[^\\/:*?"<>|\r\n]+){2,}(?:\\[^\\/:*?"<>|\r\n]+\.\w+)?)");
	std::regex dll_pattern(R"(^.+\.dll$)");
	std::regex file_pattern(R"(([^\\/:*?"<>|\r\n]+)\.(exe|bat|cmd|vbs|txt|log|ini|reg|msi|sys|inf|drv|com|cpl|scr|hlp|ico|lnk))", std::regex::icase);

	for (const std::string& str : excartStrings)
	{
		if (WIN_API.count(str))
			p->foundAPI.push_back(str);

		if (FUNCTIONS_LIST.count(str))
			p->foundFunctions.push_back(str);

		if (CMD_COMMAND_LIST.count(str))
			p->foundCommands.push_back(str);

		if (regex_search(str, url_pattern))
			p->foundURL.push_back(str);

		if (regex_search(str, ip_pattern))
			p->foundIP.push_back(str);

		if (regex_search(str, path_pattern))
			p->foundPath.push_back(str);

		if (regex_search(str, dll_pattern))
			p->foundDLL.push_back(str);

		if (regex_search(str, file_pattern))
			p->foundFile.push_back(str);
	}
	//for (const std::string& str : excartStrings)
	//{
	//	if (find(WIN_API.begin(), WIN_API.end(), str) != WIN_API.end())
	//		p->foundAPI.push_back(str);

	
	 p->count = p->foundFile.size() + p->foundDLL.size()+ p->foundURL.size() + p->foundAPI.size() + p->foundPath.size()+ p->foundIP.size() +p->foundCommands.size() + p->foundFunctions.size();
	return p;
}
PEFunction::~PEFunction()
{
}

PEFunction::PEFunction(const PEFunction& other)
{
	this->foundAPI = other.foundAPI;
	this->foundFunctions = other.foundFunctions;
	this->foundURL = other.foundURL;
	this->foundDLL = other.foundDLL;
	this->foundIP = other.foundIP;
	this->foundPath = other.foundPath;
	this->foundFile = other.foundFile;
	this->foundCommands = other.foundCommands;
	this->count = other.count;
}


