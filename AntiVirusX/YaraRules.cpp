#include "YaraRules.h"
bool LoadCompilerRules(const std::string& rulesPath, YR_COMPILER** compiler, YR_RULES** rules)
{
	if (yr_compiler_create(compiler) != ERROR_SUCCESS) {
		std::cerr << "Failed to create Static YARA compiler" << std::endl;
		yr_finalize();
		return false;
	}
	FILE* rule_file = nullptr;
	wprintf(L"Trying to open file: %s\n", rulesPath.c_str());

	if (fopen_s(&rule_file, rulesPath.c_str(), "r") != 0) {
		wprintf(L"Failed to open file: %s\n", rulesPath.c_str());
	}
	else {
		wprintf(L"File opened successfully.\n");
		// עיבוד הקובץ כאן
		

	/*FILE* rule_file = nullptr;
	fopen_s(&rule_file, rulesPath.c_str(), "r");*/
	if (!rule_file) {
		std::cerr << "Failed to open YARA rule file: " << Global::filePathYaraRulesDynamic.c_str() << std::endl;
		yr_compiler_destroy(*compiler);
		return false;
	}

	int errors = yr_compiler_add_file(*compiler, rule_file, nullptr, Global::filePathYaraRulesDynamic.c_str());
	fclose(rule_file);
	if (errors > 0) {
		std::cerr << "YARA compilation failed with " << errors << " errors" << std::endl;
		yr_compiler_destroy(*compiler);
		return false;
	}

	if (yr_compiler_get_rules(*compiler, rules) != ERROR_SUCCESS) {
		std::cerr << "Failed to get compiled rules" << std::endl;
		yr_compiler_destroy(*compiler);
		return false;
	}
	
	fclose(rule_file); // אל תשכח לסגור את הקובץ בסיום
	}return true;
}

YaraRules::YaraRules() {
	if (yr_initialize() != ERROR_SUCCESS) {
		Global::objLog->WriteToLog(LogLevel::WARNING, "Failed to initialize YARA");
		this->stateAnalysis = UNSUCCESSFUL;
	}

}
YaraRules::YaraRules(const YaraRules& other)
{
	this->rulesPath = other.rulesPath;
	this->stateAnalysis = other.stateAnalysis;


	this->compilerDynamic = nullptr;
	this->rulesDynamic = nullptr;
	this->compilerStatic = nullptr;
	this->rulesStatic = nullptr;
}


YaraRules::~YaraRules()
{
	Stop();
}
void YaraRules::Stop() {

	if (rulesStatic != nullptr)
		yr_rules_destroy(rulesStatic);
	if (compilerStatic != nullptr) {
		yr_compiler_destroy(compilerStatic);
	}
	if (rulesDynamic != nullptr)
		yr_rules_destroy(rulesDynamic);
	if (compilerDynamic != nullptr) {
		yr_compiler_destroy(compilerDynamic);
		yr_finalize();
	}
}
StateAnalysis YaraRules::ScanStaticMatches(const std::vector<std::string>& inputStrings)
{
	if (!stateAnalysis || rulesDynamic == nullptr) {
		std::cerr << "YARA rules not loaded." << std::endl;
		return UNSUCCESSFUL;
	}

	struct CallbackData {
		bool matched = false;
	} cbData;

	auto callback = [](int message, void* message_data, void* user_data) -> int {
		if (message == CALLBACK_MSG_RULE_MATCHING) {
			YR_RULE* rule = static_cast<YR_RULE*>(message_data);
			std::cout << "Matched rule: " << rule->identifier << std::endl;

			CallbackData* data = static_cast<CallbackData*>(user_data);
			data->matched = true;
		}
		return CALLBACK_CONTINUE;
		};

	this->stateAnalysis = HARMELLS;

	std::string mergedInput;
	for (const auto& str : inputStrings) {
		mergedInput += str + "\n"; 
	}

	cbData.matched = false;

	int result = yr_rules_scan_mem(
		rulesDynamic,
		reinterpret_cast<uint8_t*>(&mergedInput[0]), mergedInput.size(),
		0,         
		callback,
		&cbData,   
		0          
	);

	if (result != ERROR_SUCCESS) {
		std::cerr << "Error scanning merged input." << std::endl;
	}
	else if (cbData.matched) {
		std::cout << "Match found in merged input." << std::endl;
		Global::objLog->WriteToLog(LogLevel::INFO, "Match found in merged input");
		this->stateAnalysis = VIRUS_YARA;
	}

	return this->stateAnalysis;
}


StateAnalysis YaraRules::StartDynamic(std::vector<std::string> strings)
{
	if (!LoadCompilerRules(Global::filePathYaraRulesDynamic, &this->compilerDynamic, &this->rulesDynamic))
		return UNSUCCESSFUL;
	Global::objLog->WriteToLog(LogLevel::INFO, Global::Scanning + "Yara Rules ");

	return ScanStaticMatches(strings);

}
StateAnalysis YaraRules::Match_XMRIG_Miner_Rule(const std::vector<std::string>& strings)
{
	if (!stateAnalysis || rulesStatic == nullptr) {
		std::cerr << "YARA rules not loaded." << std::endl;
		return UNSUCCESSFUL;
	}

	struct CallbackData {
		bool matched = false;
	} cbData;

	auto callback = [](int message, void* message_data, void* user_data) -> int {
		if (message == CALLBACK_MSG_RULE_MATCHING) {
			YR_RULE* rule = static_cast<YR_RULE*>(message_data);
			std::cout << "Matched rule: " << rule->identifier << std::endl;

			CallbackData* data = static_cast<CallbackData*>(user_data);
			data->matched = true;
		}
		return CALLBACK_CONTINUE;
		};

	this->stateAnalysis = HARMELLS;
	std::string mergedInput;
	for (const auto& str : strings) {
		mergedInput += str + "\n"; 
	}

	cbData.matched = false;

	int result = yr_rules_scan_mem(
		rulesDynamic,
		reinterpret_cast<uint8_t*>(&mergedInput[0]), mergedInput.size(),
		0,         
		callback,
		&cbData,   
		0          
	);

	if (result != ERROR_SUCCESS) {
		std::cerr << "Error scanning merged input." << std::endl;
	}
	else if (cbData.matched) {
		std::cout << "Match found in merged input." << std::endl;
		Global::objLog->WriteToLog(LogLevel::INFO, "Match found in merged input");
		this->stateAnalysis = VIRUS_YARA;
	}

	return this->stateAnalysis;
}

StateAnalysis YaraRules::StartStatic(std::vector<std::string> peFunction)
{
	if (!LoadCompilerRules(Global::filePathYaraRulesStatic, &this->compilerStatic, &this->rulesStatic))
		return UNSUCCESSFUL;
	Global::objLog->WriteToLog(LogLevel::INFO, Global::Scanning + "Yara Static Rules ");

	return Match_XMRIG_Miner_Rule(peFunction);
}