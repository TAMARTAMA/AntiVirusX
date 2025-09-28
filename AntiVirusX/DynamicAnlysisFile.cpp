#include "DynamicAnlysisFile.h"

HyperV* DynamicAnlysisFile::VM = new HyperV(Global::vmName, Global::filePathDestionVM, Global::checkPoint, Global::filePathCredVM);
std::mutex DynamicAnlysisFile::criticalSectionMutex;
DynamicAnlysisFile::DynamicAnlysisFile(FileExe fileExe)
{
	this->fileExe = fileExe;
	this->fileExe.SetStateFile(StateFile::ANLAYSIS_DYNAMIC);
	Global::objLog->WriteToLog(LogLevel::INFO, "file " + fileExe.wstringToStringF(fileExe.GetPathFile()) + " start dynamic anlaysis");
}

bool DynamicAnlysisFile::Start()
{
	std::lock_guard<std::mutex> lock(criticalSectionMutex);
	if (!VM->RestoreVM())
		return false;
	if (!VM->StartVM())
		return false;
	if (!VM->CopyFileHostToVM(this->fileExe.GetPathFile(),Global::filePathDestionVM))
		return false;
	if (!VM->RunFileInVM())
		return false;
	if (!VM->ReadLogFile())
		return false;
	if(!VM->StopVM())
		return false;
	if(!VM->RestoreVM())
		return false;
	return true;
}



std::vector<std::string> DynamicAnlysisFile::ExtractEventNames(const std::wstring& filePath) {
	std::ifstream file(filePath);
	std::vector<std::string> eventNames;
	std::string line;

	while (std::getline(file, line)) {
		std::size_t pos = line.find("Event Name:");
		if (pos != std::string::npos) {
			std::string eventName = line.substr(pos + std::string("Event Name:").length());
			size_t start = eventName.find_first_not_of(" \t");
			if (start != std::string::npos)
				eventName = eventName.substr(start);
			eventNames.push_back(eventName);
		}
	}

	return eventNames;
}
