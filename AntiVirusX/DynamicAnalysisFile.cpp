#include "DynamicAnalysisFile.h"


DynamicAnalysisFile::DynamicAnalysisFile(FileExe fileExe) {
	this->fileExe = fileExe;
	this->fileExe.SetStateFile(StateFile::ANLAYSIS_DYNAMIC);
}
bool DynamicAnalysisFile::Start() {
	if (ConnectVM())
		return true;
	return false;
}