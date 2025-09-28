#pragma once

#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>
#include <functional>
#include "Error.h"
#include "FileExe.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte

enum StateAnalysis
{
	UNSUCCESSFUL=0,
	HARMELLS,
	IN_REPOSITORY,
	VIRUS_YARA,
	VIRUS_STRINGS
	, UNKNOWN
};
class Global {
public:
	static Error_* objLog;
	static std::function<void(std::string)> callbackToShowMessenge;
	static bool running;
	static std::wstring vmName;
	static std::wstring fileLogAntiVirusX;
	static std::wstring filePathDestionVM;
	static std::wstring fileLogSourceVM;
	static std::wstring fileLogDesHost;
	static std::wstring filePathCredVM ;
	static std::wstring checkPoint ;	
	static std::string filePathYaraRulesDynamic;
	static std::string filePathYaraRulesStatic;
	static std::wstring fileETWMonitor;

	static std::string Failed;
	static std::string Success;
	static std::string Error_a;
	static std::string Virus;
	static std::string Start;
	static std::string Scanning;
	static std::string Anlysis;
	static std::string File;
	static std::string AntiVirusX;
	static std::string Info;
	 static  std::string wstringToStringG(const std::wstring& wstr);
	~Global();
	Global(std::function<void(std::string)> callbackToShowMessenge);
	static void SetRunning();
};


#endif // GLOBAL_H
