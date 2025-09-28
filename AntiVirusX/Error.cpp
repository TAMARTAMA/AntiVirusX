#include "Error.h"
Error_::Error_(std::wstring s)
{
	this->filename = s;
}
bool Error_::WriteToLog(LogLevel levelError, std::string message) {
    std::ofstream logFile(this->filename.c_str(), std::ios::app); // מצב append
    if (!logFile.is_open()) {
        std::cout << "Unable to open log file!" << std::endl;
        return false;
    }
    logFile << "[" << getCurrentTime() << "] "
        << "[" << logLevelToString(levelError) << "] "
        << message << std::endl;
    return true;
}

std::string Error_::logLevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR_: return "ERROR";
    case LogLevel::CRITICAL: return "CRITICAL";
    default: return "UNKNOWN";
    }
}

std::string Error_::getCurrentTime()
{
    std::time_t now = std::time(nullptr);
    std::tm tmStruct;
    localtime_s(&tmStruct, &now); 
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmStruct);
    return std::string(buf);
}

