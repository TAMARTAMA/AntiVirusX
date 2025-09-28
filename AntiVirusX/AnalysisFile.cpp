#include "AnalysisFile.h"


AnalysisFile::AnalysisFile(FileExe fileExe)
{
	this->fileExe = fileExe;
	this->fileExe.SetStateFile(StateFile::ANLAYSIS);
}
StateAnalysis AnalysisFile::Start() {
	StateAnalysis stateAnalysisStatic =UNSUCCESSFUL;
	StateAnalysis stateAnalysisDynamic =UNSUCCESSFUL;
	StaticAnalysisFile staticAnalysisFile(fileExe);
	stateAnalysisStatic = staticAnalysisFile.Start();

	DynamicAnlysisFile dynamicAnlysisFile(fileExe);
	if (!dynamicAnlysisFile.Start())
		 stateAnalysisDynamic= UNSUCCESSFUL;
	else
		stateAnalysisDynamic = UNKNOWN;
	Algorithm algorithm(&staticAnalysisFile,&dynamicAnlysisFile);
	return algorithm.Start();
}
