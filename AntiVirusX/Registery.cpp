#include "Registery.h"

bool Registery::IsAutoRunEnabled(const std::wstring& appName) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return false;

    DWORD size = 0;
    LONG result = RegQueryValueExW(hKey, appName.c_str(), nullptr, nullptr, nullptr, &size);
    RegCloseKey(hKey);
    return (result == ERROR_SUCCESS);
}

bool Registery::EnableAutoRun(const std::wstring& appName, const std::wstring& appPath) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return false;

    LONG result = RegSetValueExW(hKey, appName.c_str(), 0, REG_SZ,
        reinterpret_cast<const BYTE*>(appPath.c_str()),
        static_cast<DWORD>((appPath.length() + 1) * sizeof(wchar_t)));

    RegCloseKey(hKey);
    return (result == ERROR_SUCCESS);
}

bool Registery::DisableAutoRun(const std::wstring& appName) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return false;

    LONG result = RegDeleteValueW(hKey, appName.c_str());
    RegCloseKey(hKey);
    return (result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND);
}
