#pragma once
#ifndef Error_H
#define Error_H
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
enum class LogLevel {
	INFO,
	WARNING,
	ERROR_,
	CRITICAL
};
class Error_
{
private:
	std::wstring filename;
public:
	Error_(std::wstring);
	bool WriteToLog(LogLevel, std::string);
	std::string logLevelToString(LogLevel );
	std::string getCurrentTime();

};

#endif // Error_H


