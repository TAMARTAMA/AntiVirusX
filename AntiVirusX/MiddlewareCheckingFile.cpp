
#include "MiddlewareCheckingFile.h"
MiddlewareCheckingFile::MiddlewareCheckingFile(FileExe fileExe)
{
	this->fileExe = fileExe;
	this->fileExe.SetStateFile(StateFile::CHECKING);
	if (isExecutablePE()) {
		Global::objLog->WriteToLog(LogLevel::INFO, fileExe.wstringToStringF(fileExe.GetPathFile()) + " an executable");
		StateAnalysis state = RunAnlysisFile();
		switch (state)
		{
		case UNSUCCESSFUL:
			Global::callbackToShowMessenge("���� ����� ������ ����� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ����� ������ ����.");
			break;
		case HARMELLS:
			Global::callbackToShowMessenge("���� ���� ��� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ����� ������ ����.");
			break;
		case IN_REPOSITORY:
			Global::callbackToShowMessenge("����� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ���� ����� ��������� ����.");
			break;
		case VIRUS_YARA:
			Global::callbackToShowMessenge("����� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ���� �� ��� ���� ���� .");
			break;
		case VIRUS_STRINGS:
			Global::callbackToShowMessenge("����� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ���� �� ��� ������� ������.");
			break;
		case UNKNOWN:
			Global::callbackToShowMessenge("����� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ���� ��� ����. �� ����� �� ����� ����� ����.");
			break;
		default:
			Global::callbackToShowMessenge("����� " + Global::wstringToStringG(this->fileExe.GetPathFile()) + " ���� ��� ����. �� ����� �� ����� ����� ����.");
			break;
		}
	}
	else{
		Global::objLog->WriteToLog(LogLevel::INFO, fileExe.wstringToStringF(fileExe.GetPathFile()) + " not an executable");
	}
}

bool MiddlewareCheckingFile::isExecutablePE() {
	std::ifstream file(this->fileExe.GetPathFile(), std::ios::binary);
	if (!file) return false;

	char mz[2];
	file.read(mz, 2);
	if (mz[0] != 'M' || mz[1] != 'Z') return false;  

	file.seekg(0x3C);  
	int pe_offset = 0;
	file.read(reinterpret_cast<char*>(&pe_offset), sizeof(pe_offset));

	file.seekg(pe_offset);
	char pe[4];
	file.read(pe, 4);

	return (pe[0] == 'P' && pe[1] == 'E' && pe[2] == 0 && pe[3] == 0);

}
StateAnalysis MiddlewareCheckingFile::RunAnlysisFile() {
	AnalysisFile anlysis(this->fileExe);
	 
	return anlysis.Start();
}
