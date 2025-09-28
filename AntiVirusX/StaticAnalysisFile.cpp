#include "StaticAnalysisFile.h"
StaticAnalysisFile::StaticAnalysisFile(FileExe& fileExe) {
	this->fileExe = fileExe;
	this->fileExe.SetStateFile(StateFile::ANLAYSIS_STATIC);
	Global::objLog->WriteToLog(LogLevel::INFO, "file " + fileExe.wstringToStringF(fileExe.GetPathFile()) + " start static anlaysis");

}
StaticAnalysisFile::~StaticAnalysisFile() {
}
StateAnalysis StaticAnalysisFile::Start() {
	std::wstring filepath = this->fileExe.GetPathFile();
	StateAnalysis state = UNSUCCESSFUL;
	 connectVirusTotalHash = ConnectVirusTotalHash(filepath);
	 if (!connectVirusTotalHash)
		 state= IN_REPOSITORY;
	 verifySignature = VerifySignature(filepath);
	 scanBinaryFile = ScanBinaryFile(filepath);
	 state = UNKNOWN;
	 if(scanBinaryFile.get()->count/scanBinaryFile.get()->countAllStrings>0.05)
		 state = VIRUS_STRINGS;
	 
	return state;
}

std::shared_ptr<PEFunction> StaticAnalysisFile::GetScanBinaryFile()
{
	return this->scanBinaryFile;
}
