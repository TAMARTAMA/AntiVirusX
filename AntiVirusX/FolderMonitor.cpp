#include "FolderMonitor.h"

FolderMonitor::~FolderMonitor() {
	stop();
}
void FolderMonitor::stop() {

	if (overlapped.hEvent != NULL) {
		CloseHandle(overlapped.hEvent);
		overlapped.hEvent = NULL;
	}

	if (hDir != INVALID_HANDLE_VALUE) {
		CloseHandle(hDir);
		hDir = INVALID_HANDLE_VALUE;
	}
}
void FolderMonitor::StartMonitoring(std::function<void(FileExe fileExe)> callbackToScan) {
	this->callbackToScan = callbackToScan;
	if (getDownloadsPath()) {
		if (MonitorFolder()) {
			Global::objLog->WriteToLog(LogLevel::INFO, " monitoring user's downloads ");
			Start();
		}
		else {
			Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to monitor downloads folder !");
		}
	}
	else {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to get downloads path !");
	}

}
bool FolderMonitor::getDownloadsPath() {
	PWSTR path = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path);

	if (SUCCEEDED(hr)) {
		monitorPath = path;
		CoTaskMemFree(path);
		return true;
	}
	Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to get path downloads !");
	return false;
}
bool FolderMonitor::MonitorFolder() {
	hDir = CreateFileW(
		monitorPath.c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
	);


	if (hDir == INVALID_HANDLE_VALUE) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to open directory downloads  Error: " + GetLastError());
		return false;
	}
	overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlapped.hEvent == NULL) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to create event Error: " + GetLastError());
		CloseHandle(hDir);
		hDir = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}
void FolderMonitor::Start()
{
	while (Global::running) {
		BOOL success = ReadDirectoryChangesW(
			hDir,
			buffer,
			sizeof(buffer),
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
			&bytesReturned,
			&overlapped,
			NULL
		);

		if (!success) {
			std::cerr << "ReadDirectoryChangesW failed. Error: " << GetLastError() << std::endl;
			break;
		}

		DWORD waitStatus = WaitForSingleObject(overlapped.hEvent, INFINITE);
		if (waitStatus != WAIT_OBJECT_0) {
			std::cerr << "Wait failed. Error: " << GetLastError() << std::endl;
			break;
		}

		DWORD bytesTransferred;
		if (!GetOverlappedResult(hDir, &overlapped, &bytesTransferred, FALSE)) {
			std::cerr << "GetOverlappedResult failed. Error: " << GetLastError() << std::endl;
			break;
		}

		ResetEvent(overlapped.hEvent);

		if (bytesTransferred > 0) {
			processNotifications(bytesTransferred);
		}
	}

}
bool FolderMonitor::isTemporaryFile(const std::wstring& filename) {
	static const std::vector<std::wstring> tempExts = {
		L".part", L".crdownload", L".download", L".partial", L".tmp"
	};

	for (const auto& ext : tempExts) {
		if (filename.length() >= ext.length() &&
			filename.compare(filename.length() - ext.length(), ext.length(), ext) == 0) {
			return true;
		}
	}

	return false;
}
void FolderMonitor::processNotifications(DWORD bytesTransferred) {
	FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)buffer;

	do {
		/*std::wstring filename(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));
		if ((pNotify->Action == FILE_ACTION_ADDED || pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME) &&
			!isTemporaryFile(filename)) {
			if (filename[0] != L'.') {
				std::wstring fullPath = monitorPath + L"\\" + filename;
				DWORD attributes = GetFileAttributesW(fullPath.c_str());
				if (attributes != INVALID_FILE_ATTRIBUTES &&
					!(attributes & FILE_ATTRIBUTE_DIRECTORY) &&
					!(attributes & FILE_ATTRIBUTE_SYSTEM) &&
					!(attributes & FILE_ATTRIBUTE_HIDDEN)) {
					HANDLE hFile = CreateFileW(
						fullPath.c_str(),
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL
					);

					LARGE_INTEGER fileSize;
					fileSize.QuadPart = 0;

					if (hFile != INVALID_HANDLE_VALUE) {
						GetFileSizeEx(hFile, &fileSize);
						CloseHandle(hFile);


					}
					std::thread([fullPath, this]() {
						this->callbackToScan(FileExe(fullPath));
						}).detach();
				}
			}
		}*/
		std::wstring filename(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));

		bool isRelevantAction = pNotify->Action == FILE_ACTION_ADDED || pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME;
		if (!isRelevantAction || isTemporaryFile(filename) || filename[0] == L'.') {
			return;
		}

		std::wstring fullPath = monitorPath + L"\\" + filename;
		DWORD attributes = GetFileAttributesW(fullPath.c_str());

		bool isValidFile =
			attributes != INVALID_FILE_ATTRIBUTES &&
			!(attributes & FILE_ATTRIBUTE_DIRECTORY) &&
			!(attributes & FILE_ATTRIBUTE_SYSTEM) &&
			!(attributes & FILE_ATTRIBUTE_HIDDEN);

		if (!isValidFile) {
			return;
		}

		HANDLE hFile = CreateFileW(
			fullPath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile != INVALID_HANDLE_VALUE) {
			LARGE_INTEGER fileSize;
			if (GetFileSizeEx(hFile, &fileSize)) {
			}
			CloseHandle(hFile);
		}

		std::thread([fullPath, this]() {
			this->callbackToScan(FileExe(fullPath));
			}).detach();


		if (pNotify->NextEntryOffset == 0)
			break;

		pNotify = (FILE_NOTIFY_INFORMATION*)((unsigned char*)pNotify + pNotify->NextEntryOffset);

	} while (Global::running);
}
