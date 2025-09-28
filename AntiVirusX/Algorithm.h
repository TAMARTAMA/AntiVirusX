#pragma once
#ifndef ALGORITHM_H
#define ALGORITHM_H_H
#include "StaticAnalysisFile.h"
#include "DynamicAnlysisFile.h"
#include "YaraRules.h"
class Algorithm
{
private:
	StaticAnalysisFile* staticAnalysisFile;
	DynamicAnlysisFile* dynamicAnlysisFile;
public:
	static std::optional<YaraRules> yara;

	Algorithm(StaticAnalysisFile* staticAnalysisFile, DynamicAnlysisFile* dynamicAnlysisFile);
	~Algorithm();
	StateAnalysis Start();
	bool AnalysisResult();
};

#endif //ALGORITHM_H