#pragma once
#pragma once
#include <Windows.h>
#include <string>

class Registery {
public:
    static bool IsAutoRunEnabled(const std::wstring& appName);
    static bool EnableAutoRun(const std::wstring& appName, const std::wstring& appPath);
    static bool DisableAutoRun(const std::wstring& appName);
};


