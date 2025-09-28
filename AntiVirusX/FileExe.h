#pragma once
#include <string>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte

#ifndef FILE_EXE_H
#define FILE_EXE_H
enum StateFile
{
	MONITORING,
	CHECKING,
	NOT_VALID,
	ANLAYSIS,
	ANLAYSIS_STATIC,
	ANLAYSIS_DYNAMIC,
	IDENTFIFY_VIRUS,
	IDENTFIFY_LEGITIMIC,

};
class FileExe
{
private:
	StateFile stateFile;
	std::wstring filePath;
public:

	//public:
	FileExe(std::wstring filePath);
	FileExe();
	void SetStateFile(StateFile stateFile);
	StateFile GetStateFile();
	void SetPathFile(std::wstring filePath);
	std::wstring GetPathFile();
	std::string wstringToStringF(const std::wstring& wstr);
	std::wstring operator*(const FileExe& fileExe);
};

#endif //FILE_EXE_H