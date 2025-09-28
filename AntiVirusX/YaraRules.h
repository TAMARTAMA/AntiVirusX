#pragma once
//#ifndef YRAR_RULES_H
//#define YRAR_RULES_H
//
//
//
////#ifndef module
////#undef module
////#endif
//#include <string>
//#include <memory>
//#include <cstdio>
//#include <yara.h>
//#include "Global.h"
//#include "PEFunction.h"
#pragma once

#ifndef YARA_RULES_H
#define YARA_RULES_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <string>
#include <memory>
#include <cstdio>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <yara.h>
#include "Global.h"
#include "PEFunction.h"


class YaraRules
{
private:
	StateAnalysis stateAnalysis;
	std::string rulesPath=Global::filePathYaraRulesDynamic;
	YR_COMPILER* compilerDynamic=nullptr;
	YR_RULES* rulesDynamic = nullptr;
	YR_COMPILER* compilerStatic = nullptr;
	YR_RULES* rulesStatic = nullptr;
	StateAnalysis ScanStaticMatches(const std::vector<std::string>& );

public:
	YaraRules();
	~YaraRules();
	void Stop();
	StateAnalysis StartDynamic(std::vector<std::string>);
	StateAnalysis Match_XMRIG_Miner_Rule(const std::vector<std::string>& strings);
	StateAnalysis StartStatic(std::vector<std::string>);
	YaraRules(const YaraRules& other); 

};

#endif // !YRAR_RULES_H