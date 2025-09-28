#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <algorithm>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte
using namespace std;





//const std::vector<std::string> MaliciousFunctions = {
//	"Create", "File", "Delete", "Get", "Mode", "Console", "Output",
//	"Heap", "DC", "Release", "Write", "Alloc", "Size", "of", "Cmd",
//	"cmd", "Type", "Handle", "Close", "Load", "Free", "library",
//	"Process", "Module", "Exit", "Local", "Thread", "System", "Byte",
//	"Open", "Current", "Id", "Find", "Next", "Second", "First", "Device",
//	"Set", "Run", "Pen", "Value", "Virtual", "Protect", "Command",
//	"Environment", "mouse", "Mouse", "Key", "key", "Manager", "Icon",
//	"Cursor", "Draw", "draw", "hook", "BitBlt", "Select", "Object",
//	"Stretch", "Gen", "Crypt", "Cryptor", "Crypto", "Look", "Token",
//	"Head", "Out", "Prepare", "Name", "@", "#", "_", "?", "*"
//};
//double ScanImportsFile( string& fileName);
//string changeExtensionToDll( string& programPath);
//void changeExtensionBack( string& programPath);
//bool IsMaliciousFunction(const std::string& functionName);
//double ScanPEFile(const std::string& fileName);
