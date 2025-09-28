#pragma once
#ifndef DYNAMI_ANLYSIS_h
#define DYNAMI_ANLYSIS_h
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Error.h"
#include "Global.h"
#include "HyperV.h"
#include <mutex>
class DynamicAnlysisFile
{

private:
	FileExe fileExe;
	std::wstring filePathLogMonitor=Global::fileLogDesHost;

public:
	static std::mutex criticalSectionMutex;
	static HyperV* VM;
	DynamicAnlysisFile(FileExe);
	bool Start();
	std::vector<std::string> ExtractEventNames(const std::wstring& filePath);
};
#endif //DYNAMI_ANLYSIS_h
