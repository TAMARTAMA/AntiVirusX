//#pragma once
//#include <windows.h>
//#include <detours.h>
//#include <iostream>
//#include <fstream>
//
//#include <filesystem>
//// מצביע לפונקציה המקורית
//typedef HRESULT(WINAPI* URLDownloadToFile_t)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);
//URLDownloadToFile_t OriginalURLDownloadToFile = nullptr;
//#pragma comment(lib, "detours.lib")//מחבר את הספרייה של Detours לקובץ ה-EXE
//using namespace std;
//namespace fs = std::filesystem;
//class DownloadsMonitor
//{
//	const std::wstring TARGET_FOLDER = L"C:\\Users\\USER\\Downloads";
//	// פונקציה מחוברת (Hook)
//	HRESULT WINAPI HookedURLDownloadToFile(LPUNKNOWN pCaller, LPCWSTR szURL, LPCWSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB) {
//		wcout << L"File download detected: " << szFileName << endl;
//
//		// המתן עד שההורדה תושלם
//		Sleep(1000);
//
//		// העברת הקובץ לתיקייה אחרת
//		wstring fileName = fs::path(szFileName).filename();
//		wstring targetPath = TARGET_FOLDER + L"\\" + fileName;
//
//		if (MoveFile(szFileName, targetPath.c_str())) {
//			wcout << L"File moved to: " << targetPath << endl;
//		}
//		else {
//			wcout << L"Failed to move file. Error: " << GetLastError() << endl;
//		}
//
//		return OriginalURLDownloadToFile(pCaller, szURL, szFileName, dwReserved, lpfnCB);
//	}
//};
//
////// פונקציה שמחברת את ההוק
////void HookAPI() {
////	HMODULE hModule = GetModuleHandle(L"urlmon.dll");
////	if (hModule) {
////		OriginalURLDownloadToFile = (URLDownloadToFile_t)GetProcAddress(hModule, "URLDownloadToFileW");
////		if (OriginalURLDownloadToFile) {
////			DetourTransactionBegin();
////			DetourUpdateThread(GetCurrentThread());
////			DetourAttach(&(PVOID&)OriginalURLDownloadToFile, HookedURLDownloadToFile);
////			DetourTransactionCommit();
////			wcout << L"Hook attached to URLDownloadToFileW!" << endl;
////		}
////	}
////}
////
////int main() {
////	wcout << L"Starting download monitor..." << endl;
////	DetourRestoreAfterWith();
////	HookAPI();
////
////	// שמור את התוכנית פעילה כדי שההוק ימשיך לעבוד
////	while (true) {
////		Sleep(1000);
////	}
////
////	return 0;
////}
////
