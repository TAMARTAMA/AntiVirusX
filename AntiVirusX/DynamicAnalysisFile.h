#pragma once
#include "FileExe.h"
//#include <curl/curl.h>
#ifndef DynamicAnalysisFile_H
#define DynamicAnalysisFile_H
class DynamicAnalysisFile
{
private:
	FileExe fileExe;
public:
	DynamicAnalysisFile(FileExe);
	bool Start();
	bool ConnectVM() {
		/*CURL* hnd = curl_easy_init();

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);
		curl_easy_setopt(hnd, CURLOPT_URL, "https://www.virustotal.com/api/v3/files/id");

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "accept: application/json");
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

		CURLcode ret = curl_easy_perform(hnd);*/
		return true;
	}
};
#endif //StaticAnalysisFile_H
