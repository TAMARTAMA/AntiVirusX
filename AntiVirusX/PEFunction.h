#pragma once
#ifndef PE_FUNCTION_H
#define PE_FUNCTION_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <algorithm>
#include <regex>
#include "Global.h"
#include <unordered_set>

class PEFunction {
public:
    std::vector<std::string> foundAPI;
    std::vector<std::string> foundFunctions;
    std::vector<std::string> foundURL;
    std::vector<std::string> foundDLL;
    std::vector<std::string> foundIP;
    std::vector<std::string> foundPath;
    std::vector<std::string> foundFile;
    std::vector<std::string> foundCommands;
    int count=0;
    int countAllStrings=0;
    void SetCount(int count);
    int GetCount();
    PEFunction(const PEFunction& other); // בנאי העתקה
    ~PEFunction(); // מפנה
    PEFunction()=default; // מפנה

    //friend PEFunction* ScanBinaryFile(const std::string& filePath);
};
std::shared_ptr<PEFunction> ScanBinaryFile(const std::wstring& filePath);
 #endif // PE_FUNCTION_H


