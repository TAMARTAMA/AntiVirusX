#include "ConnectVirusTotal.h"  
#pragma comment (lib,"Iphlpapi.lib")

std::string CalculateFileHash(const std::wstring& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
	{
		Global::objLog->WriteToLog(LogLevel::ERROR_, "Cannot open file " + Global::wstringToStringG(filePath));
		return "";
	}
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	char buffer[8192];
	while (file.read(buffer, sizeof(buffer)))
	{
		SHA256_Update(&sha256, buffer, file.gcount());
	}
	SHA256_Update(&sha256, buffer, file.gcount());
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_Final(hash, &sha256);
	std::ostringstream hashString;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		hashString << std:: hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return hashString.str();
}

DetectionVT analyze_virustotal_response(const std::string& response) {
	try {
		auto json_response = json::parse(response);

		if (!json_response.contains("data")) {
			std::cerr << "Invalid response: missing 'data' field." << std::endl;
			return HARMLESS;
		}

		auto attributes = json_response["data"]["attributes"];
		std::string file_name = attributes["meaningful_name"];
		int size = attributes["size"];
		std::string type = attributes["type_description"];
		std::string last_analysis_date = attributes["last_analysis_date"].is_number()
			? std::to_string(attributes["last_analysis_date"].get<long>())
			: "N/A";

		int malicious = attributes["last_analysis_stats"]["malicious"];
		int suspicious = attributes["last_analysis_stats"]["suspicious"];
		int undetected = attributes["last_analysis_stats"]["undetected"];
		int harmless = attributes["last_analysis_stats"]["harmless"];

		std::cout << "\n===== VirusTotal Scan Report =====\n";
		std::cout << "File Name: " << file_name << "\n";
		std::cout << "File Size: " << size << " bytes\n";
		std::cout << "Type: " << type << "\n";
		std::cout << "Scan Date (Unix Timestamp): " << last_analysis_date << "\n";

		std::cout << "\n--- Detection Summary ---\n";
		std::cout << "Malicious: " << malicious << "\n";
		std::cout << "Suspicious: " << suspicious << "\n";
		std::cout << "Undetected: " << undetected << "\n";
		std::cout << "Harmless: " << harmless << "\n";

		std::cout << "\n--- Per Engine Results ---\n";

		std::set<std::string> threat_categories_found;

		auto results = attributes["last_analysis_results"];
		for (auto& [engine, result] : results.items()) {
			std::string category = result["category"];
			if (category == "malicious" || category == "suspicious") {
				std::string result_str = result["result"].is_null() ? "Unknown" : result["result"].get<std::string>();
				std::cout << "[!] " << engine << ": " << result_str << " (" << category << ")\n";
				std::string lower_result = result_str;
				std::transform(lower_result.begin(), lower_result.end(), lower_result.begin(), ::tolower);

				if (lower_result.find("trojan") != std::string::npos)
					threat_categories_found.insert("Trojan");
				if (lower_result.find("worm") != std::string::npos)
					threat_categories_found.insert("Worm");
				if (lower_result.find("spyware") != std::string::npos || lower_result.find("spy") != std::string::npos)
					threat_categories_found.insert("Spyware");
				if (lower_result.find("ransom") != std::string::npos)
					threat_categories_found.insert("Ransomware");
				if (lower_result.find("virus") != std::string::npos)
					threat_categories_found.insert("Virus");
				if (lower_result.find("adware") != std::string::npos)
					threat_categories_found.insert("Adware");
				if (lower_result.find("backdoor") != std::string::npos)
					threat_categories_found.insert("Backdoor");
			}
		}

		if (!threat_categories_found.empty()) {
			std::cout << "\n--- Threat Types Detected ---\n";
			for (const auto& type : threat_categories_found) {
				std::cout << type << "\n";
			}
		}
		else {
			std::cout << "\nNo known malware types detected in engine names.\n";
		}

		std::cout << "==================================\n";


		if (malicious >= 5) return MALICIOUS;

		if (malicious > 0 || suspicious >= 3) {
			std::cout << " חשוד - כדאי לבדוק לעומק." << std::endl;
			return SUSPICIOUS;
		}
		return HARMLESS;
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
		return NOT_CHECK;
	}
	return NOT_CHECK;
}

bool ConnectVirusTotalHash(const std::wstring filepath)
{
	DetectionVT analyze = NOT_CHECK;
	std::string Hash = CalculateFileHash(filepath);
	CURL* curl;
	std::string response;
	CURLcode res;
	curl = curl_easy_init();
	if (curl) {
		std::string url = "https://www.virustotal.com/api/v3/files/" + Hash;

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "x-apikey: " VIRUSTOTAL_API_KEY);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			Global::objLog->WriteToLog(LogLevel::ERROR_, " cURL error: " );
		}
		else {
			std::cout << "VirusTotal Response: " << response << std::endl;
			analyze = analyze_virustotal_response(response);
		}
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}
	return analyze==HARMLESS;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
