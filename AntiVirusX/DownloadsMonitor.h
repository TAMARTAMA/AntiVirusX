//#pragma once
//#include <windows.h>
//#include <detours.h>
//#include <iostream>
//#include <fstream>
//
//#include <filesystem>
//// ����� �������� �������
//typedef HRESULT(WINAPI* URLDownloadToFile_t)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);
//URLDownloadToFile_t OriginalURLDownloadToFile = nullptr;
//#pragma comment(lib, "detours.lib")//���� �� ������� �� Detours ����� �-EXE
//using namespace std;
//namespace fs = std::filesystem;
//class DownloadsMonitor
//{
//	const std::wstring TARGET_FOLDER = L"C:\\Users\\USER\\Downloads";
//	// ������� ������ (Hook)
//	HRESULT WINAPI HookedURLDownloadToFile(LPUNKNOWN pCaller, LPCWSTR szURL, LPCWSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB) {
//		wcout << L"File download detected: " << szFileName << endl;
//
//		// ���� �� ������� �����
//		Sleep(1000);
//
//		// ����� ����� ������� ����
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
////// ������� ������ �� ����
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
////	// ���� �� ������� ����� ��� ����� ����� �����
////	while (true) {
////		Sleep(1000);
////	}
////
////	return 0;
////}
////
