
#include "SignScan.h"
TCHAR* wstringToTCHAR(const std::wstring& wstr) {
    size_t len = wstr.length();
    TCHAR* tstr = new TCHAR[len + 1];
    wcscpy_s(tstr, len + 1, wstr.c_str());
    return tstr;
}
StateVerifySignature VerifySignature(const std::wstring& filePath)
{
    const WCHAR* pwszSourceFile = filePath.c_str();
    WINTRUST_FILE_INFO fileData = {};
    fileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    fileData.pcwszFilePath = pwszSourceFile;
    fileData.hFile = NULL;
    fileData.pgKnownSubject = NULL;
    WINTRUST_DATA winTrustData = {};
    winTrustData.cbStruct = sizeof(WINTRUST_DATA);
    winTrustData.dwUIChoice = WTD_UI_NONE;
    winTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    winTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    winTrustData.dwStateAction = WTD_STATEACTION_VERIFY;
    winTrustData.pFile = &fileData;

    GUID policyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    LONG status = WinVerifyTrust(NULL, &policyGUID, &winTrustData);

    winTrustData.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(NULL, &policyGUID, &winTrustData);
    return (status == ERROR_SUCCESS)? VerifySignature_: NotVerifySignature;
}