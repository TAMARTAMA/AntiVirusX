#include "Algorithm.h"
std::optional<YaraRules> Algorithm::yara = std::nullopt;
Algorithm::Algorithm(StaticAnalysisFile* staticAnalysisFile, DynamicAnlysisFile* dynamicAnlysisFile)
{
	this->staticAnalysisFile = staticAnalysisFile;
	this->dynamicAnlysisFile = dynamicAnlysisFile;
}
Algorithm::~Algorithm()
{
	staticAnalysisFile = nullptr;
	dynamicAnlysisFile = nullptr;

}
StateAnalysis Algorithm::Start()
{
	Global::objLog->WriteToLog(LogLevel::INFO, "Algorithm start");
	if (!yara.has_value())
		this->yara = YaraRules();
	StateAnalysis state= yara->StartDynamic((this->dynamicAnlysisFile)->ExtractEventNames(Global::fileLogDesHost));
	StateAnalysis state2= yara->StartStatic((this->staticAnalysisFile)->GetScanBinaryFile().get()->foundPath);
	return (state2!=VIRUS_YARA)?state:state2;
}
bool Algorithm::AnalysisResult()
{
	return false;
}
