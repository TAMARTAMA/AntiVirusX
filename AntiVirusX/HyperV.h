#pragma once
#ifndef HYPVER_H
#define HYPVER_H

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte
#include <wbemidl.h>
#include <comdef.h>
#include "Global.h"
#include <string>
#include <wbemcli.h>
#include <iostream>
#define HyperV_Turn_ON 2
#define HyperV_Turn_Off 3

class HyperV
{
private:


	 IWbemLocator* pLoc ;
	 IWbemServices* pSvc ;
	IEnumWbemClassObject* pEnumerator =nullptr;
	IWbemClassObject* pClassObject=nullptr;
	IWbemClassObject* pClassObjectFile =nullptr;
public:
	HyperV()=default;
	HyperV(std::wstring vmName, std::wstring filePathDestionVM, std::wstring checkPoint, std::wstring filePathCredVM);
	static bool Initialize();
	int CheckState();
	bool Connect();
	bool ChangeState(int newState);
	bool StartVM();
	bool RunPowerShell(std::wstring command, std::string messenge);
	bool CopyFile_(std::wstring source, std::wstring destion, std::wstring machine);

	bool CopyFileHostToVM(std::wstring source, std::wstring destion);

	bool CopyFileVMToHost(std::wstring source, std::wstring desation);

	bool SetFilePermissionsInVM(const std::wstring& filePathInVM);
	


	bool RunFileInVM();
	bool ReadLogFile();
	bool DeleteFileInVM(std::wstring filePathDelete);
	bool StopVM();
	bool RestoreVM();
	~HyperV();
};

#endif //HYPVER_H