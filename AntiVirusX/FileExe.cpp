#include "FileExe.h"


FileExe::FileExe() {
    this->filePath = L"";  
}
FileExe::FileExe(std::wstring filePath) {
    this->filePath = filePath;  
}

void FileExe::SetStateFile(StateFile stateFile) {
    this->stateFile = stateFile;  
}

StateFile FileExe::GetStateFile() {
    return this->stateFile;  
}

void FileExe::SetPathFile(std::wstring filePath) {  
    this->filePath = filePath;  
}

std::wstring FileExe::GetPathFile() {  
    return this->filePath;
}
std::string FileExe::wstringToStringF(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    return str;
}
std::wstring FileExe::operator*(const FileExe& fileExe) {
    return this->filePath;
}