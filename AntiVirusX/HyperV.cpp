#include "HyperV.h"
#pragma comment(lib, "wbemuuid.lib")
HyperV::HyperV(std::wstring vmName, std::wstring filePathDestionVM, std::wstring checkPoint, std::wstring filePathCredVM)
{
	this->pLoc = nullptr;
	this->pSvc = nullptr;

	bool running = Initialize();
}
bool HyperV::Initialize() {
	HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to initialize COM library. Error code = 0x" + hres);
		return false;
	}

	hres = CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, NULL);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to initialize security com. Error code = 0x" + hres);
		return false;
	}
	return true;
}
int HyperV::CheckState()
{
	std::wstring query = L"SELECT * FROM Msvm_ComputerSystem WHERE ElementName = '" + Global::vmName + L"'";
	HRESULT hres;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(query.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);
	if (FAILED(hres) || !pEnumerator) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Failed to run Query" + hres);
		return -1;
	}

	ULONG uReturn = 0;

	hres = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
	if (uReturn == 0 || FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Virtual machine not found." + hres);
		return -1;
	}

	//הדפסת ה-EnabledState (כדוגמה) https://learn.microsoft.com/en-us/windows/win32/api/wbemcli/nf-wbemcli-iwbemclassobject-get
	VARIANT vtState;
	hres = pClassObject->Get(L"EnabledState", 0, &vtState, 0, 0);
	int currentState = -1;
	if (SUCCEEDED(hres)) {
		currentState = vtState.intVal;
		Global::objLog->WriteToLog(LogLevel::INFO, "EnabledState the VM is " + vtState.intVal);
		VariantClear(&vtState);
	}

	return currentState;
}

bool HyperV::Connect() {
	HRESULT hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hres))
		return false;
	hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\virtualization\\v2"), nullptr, nullptr, 0, NULL, 0, 0, &pSvc);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to connect to WMI namespace. Error code = " + hres);
		return false;
	}
	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Could not set proxy blanket. Error code = 0x" + hres);
		return false;
	}
	return true;
}
bool HyperV::ChangeState(int newState)
{

	IWbemClassObject* pClass = NULL;
	HRESULT hres = pSvc->GetObjectW(_bstr_t(L"Msvm_ComputerSystem"), 0, NULL, &pClass, NULL);
	if (FAILED(hres))
	{
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to get Msvm_ComputerSystem class. Error code: 0x" + hres);
		return false;
	}

	VARIANT varPath;
	hres = pClassObject->Get(L"__PATH", 0, &varPath, NULL, NULL);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to get VM path." + hres);
		return false;
	}
	IWbemClassObject* pInParamsDefinition = NULL;
	hres = pClass->GetMethod(L"RequestStateChange", 0, &pInParamsDefinition, NULL);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Failed to get RequestStateChange method. Error code: 0x" + hres);
		pClassObject->Release();
		return false;
	}
	IWbemClassObject* pInParams = NULL;
	hres = pInParamsDefinition->SpawnInstance(0, &pInParams);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Failed to spawn instance for input parameters. Error code: 0x" + hres);
		pInParamsDefinition->Release();
		pClassObject->Release();
		return false;
	}
	IWbemClassObject* pOutParams = NULL;

	VARIANT varState;
	varState.vt = VT_I4;
	varState.intVal = newState;
	hres = pInParams->Put(L"RequestedState", 0, &varState, 0);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Failed to set RequestedState parameter. Error code: 0x" + hres);
		pInParams->Release();
		pInParamsDefinition->Release();
		return false;
	}
	hres = pSvc->ExecMethod(varPath.bstrVal, bstr_t(L"RequestStateChange"),
		0, NULL, pInParams, &pOutParams, NULL);
	if (FAILED(hres)) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Failed to execute RequestStateChange. Error code: 0x" + hres);
		return false;
	}
	else {
		Global::objLog->WriteToLog(LogLevel::INFO, "VM state change request sent successfully");
	}

	VariantClear(&varPath);
	pInParams->Release();
	pInParamsDefinition->Release();
	pOutParams->Release();
	return true;
}
bool HyperV::StartVM()
{

	HRESULT hres;
	if (!Connect())
		return false;

	int currentState = CheckState();
	if (currentState == 2) {
		Global::objLog->WriteToLog(LogLevel::INFO, "The VM is already running.");
		return true;
	}
	else if (currentState != 3) {

		Global::objLog->WriteToLog(LogLevel::ERROR_, "Invalid action specified state vm: " + currentState);
		return false;
	}

	return ChangeState(HyperV_Turn_ON);
}


bool HyperV::RunPowerShell(std::wstring command, std::string messenge) {

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;

	if (CreateProcess(
		NULL,
		&command[0],
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,  
		NULL,
		NULL,
		&si,
		&pi)) {

		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD exitCode;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (exitCode == 0) {
			Global::objLog->WriteToLog(LogLevel::INFO, "succses power shell the command" + messenge);
			return true;
		}
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed  power shell the command: " + messenge);
	}
	else {
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Failed to create process. Error code: " + GetLastError());
		return false;
	}
	return false;
}
bool HyperV::CopyFile_(std::wstring source, std::wstring desation, std::wstring machine)
{

	std::wstring command = L"Copy-VMFile -VMName '" + Global::vmName + L"' -SourcePath '"
		+ source + L"' -DestinationPath '" + desation + L"' -CreateFullPath -FileSource "
		+ machine;
	std::wstring fullCommand = L"powershell.exe -Command \"" + command + L"\"";
	return RunPowerShell(fullCommand, "Copy file");
}
bool HyperV::CopyFileHostToVM(std::wstring source, std::wstring desation)
{
	return CopyFile_(source, desation, L"Host");
}
bool HyperV::CopyFileVMToHost(std::wstring source, std::wstring desation)
 {

	std::wstring command =
		L"$cred = Import-Clixml -Path '" + Global::filePathCredVM + L"'; "
		L"$content = Invoke-Command -VMName '" + Global::vmName + L"' -Credential $cred "
		L"-ScriptBlock { Get-Content -Raw -Path '" + Global::fileLogSourceVM + L"' }; "
		L"$content | Out-File -FilePath '" + Global::fileLogDesHost + L"' -Encoding UTF8";

	std::wstring fullCommand = L"powershell.exe -Command \"" + command + L"\"";

	return RunPowerShell(fullCommand, "Delete file");
}


bool HyperV::RunFileInVM()
{
	std::wstring command = L"$cred = Import-Clixml -Path '" + Global::filePathCredVM + L"'; "
		L"Invoke-Command -VMName '" + Global::vmName + L"' -Credential $cred "
		L"-ScriptBlock { Start-Process -FilePath '" + Global::fileETWMonitor + L"' -Verb RunAs -WindowStyle Hidden }";

	/*std::wstring command = L"$cred = Import-Clixml -Path '" + Global::filePathCredVM + L"'; Invoke-Command -VMName '"
		+ Global::vmName + L"' -Credential $cred -ScriptBlock { & '" + Global::fileETWMonitor + L"' }";*/
	std::wstring fullCommand = L"powershell.exe -Command \"" + command + L"\"";
	return RunPowerShell(fullCommand, " run file in vm");
}


bool HyperV::ReadLogFile()
{
	return CopyFileVMToHost(Global::fileLogSourceVM, Global::fileLogDesHost);
}
bool HyperV::DeleteFileInVM(std::wstring filePathDelete)
{
	// בונה את הפקודה שתרוץ במכונה הווירטואלית
	
	std::wstring command =
		L"$cred = Import-Clixml -Path '" + Global::filePathCredVM + L"'; "
		L"Invoke-Command -VMName '" + Global::vmName + L"' -Credential $cred -ScriptBlock { "
		L"Remove-Item -Path '" + filePathDelete + L"' -Force }";
	// בונה את הפקודה המלאה להפעלה ב-PowerShell
	std::wstring fullCommand = L"powershell.exe -Command \"" + command + L"\"";

	return RunPowerShell(fullCommand, "Delete file");
}


bool HyperV::StopVM()
{
	HRESULT hres;
	if (!Connect())
		return false;

	int currentState = CheckState();
	if (currentState == 3) {
		Global::objLog->WriteToLog(LogLevel::INFO, "The VM is already stopping.");
		return true;
	}
	else if (currentState != 2) {

		Global::objLog->WriteToLog(LogLevel::ERROR_, "Invalid action specified state vm: " + currentState);
		return false;
	}

	return ChangeState(HyperV_Turn_Off);
	return false;
}

bool HyperV::RestoreVM()
{
	std::wstring command = L"Restore-VMSnapshot -VMName '" + Global::vmName + L"' -Name '" + Global::checkPoint + L"' -Confirm:$false";
	std::wstring fullCommand = L"powershell.exe -Command \"" + command + L"\"";
	return RunPowerShell(fullCommand, "Restore the vm");;

}

HyperV::~HyperV()
{
	if (pSvc)
		pSvc->Release();
	if (pLoc)
		pLoc->Release();
	if (pEnumerator)
		pEnumerator->Release();
	if (pClassObject)
		pClassObject->Release();
	if (pClassObjectFile)
		pClassObjectFile->Release();
	CoUninitialize();
	Global::objLog->WriteToLog(LogLevel::INFO, "HyperV destructor");
}
