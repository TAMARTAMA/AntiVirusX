#pragma once


#ifndef AnalysisFile_H
#define AnalysisFile_H
#include "FileExe.h"
#include "StaticAnalysisFile.h"
#include "DynamicAnlysisFile.h"
#include "Algorithm.h"
class AnalysisFile
{
private:
	FileExe fileExe;
public:
	AnalysisFile(FileExe);
	AnalysisFile() = default;
	StateAnalysis Start();
};

#endif //AnalysisFile_H