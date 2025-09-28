#pragma once
#ifndef MiddlewareCheckingFile_H
#define MiddlewareCheckingFile_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "AnalysisFile.h"
#include "FileExe.h"
class MiddlewareCheckingFile
{
private:
	FileExe fileExe;
	bool isExecutablePE();
	StateAnalysis RunAnlysisFile();
public:
	MiddlewareCheckingFile(FileExe);



};
#endif //MiddlewareCheckingFile_H
