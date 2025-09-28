#pragma once

#ifndef CONNECTION_VT_H
#define CONNECTION_VT_H
#include <string>  
#include <curl/curl.h>  
#include <set>
#include <openssl/ssl.h>  
#include <iostream>  
#include <iomanip>  
#include <fstream>  
#include <sstream>  
#include <nlohmann/json.hpp>
#include "Error.h"
#include "Global.h"
using json = nlohmann::json;
#include "FileExe.h"  
#define VIRUSTOTAL_API_KEY "2e6bf6658961281e7f4e1d69ed995b0094c3c8173cb187fff8d4266499684e0f"  
enum DetectionVT
{
	MALICIOUS,
	SUSPICIOUS,
	HARMLESS,
	NOT_CHECK
};
std::string CalculateFileHash(const std::wstring&);

bool ConnectVirusTotalHash(const std::wstring);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);


#endif // CONNECTION_VT_H