#pragma once
#ifndef StaticAnalysisFile_H
#define StaticAnalysisFile_H
#include "ConnectVirusTotal.h"
#include "SignScan.h"

#include "PEFunction.h"
#include "Global.h"
#undef byte

class StaticAnalysisFile
{
private:
	FileExe fileExe;
	bool connectVirusTotalHash ;
	StateVerifySignature verifySignature= UNSUCCESSFULVerifySignature;
	std::shared_ptr<PEFunction> scanBinaryFile =nullptr;
	
public:
	StaticAnalysisFile(FileExe&);
	StateAnalysis Start();
	std::shared_ptr<PEFunction> GetScanBinaryFile();
	~StaticAnalysisFile();

};
#endif //StaticAnalysisFile_H
