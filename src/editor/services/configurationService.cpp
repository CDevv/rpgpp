#include "services/configurationService.hpp"
#include "raylib.h"

ConfigurationService::ConfigurationService() {
	std::filesystem::path baseDir = GetWorkingDirectory();
	baseDir /= RPGPP_CONFIG_FILE;
	if (std::filesystem::exists(baseDir)) {
		this->iniFile = std::make_unique<mINI::INIFile>(baseDir.string());
		this->iniFile->read(this->iniStructure);
	} else {
		throw std::runtime_error("configuration file doesn't exist.");
	}
};

std::string ConfigurationService::getStringValue(const std::string& field, const std::string &key) {
	if (this->iniStructure[field].has(key))
		return this->iniStructure[field][key];
	std::stringstream ss;
	ss << "configuration key doesn't exist" << key;
	throw std::runtime_error(ss.str());
}

std::string ConfigurationService::getStringValue(const std::string &key) {
	return this->getStringValue(GENERAL_CONF_FIELD, key);
}

void ConfigurationService::setStringValue(const std::string& field, const std::string &key, const std::string &value) {
	this->iniStructure[field].set(key, value);
}

void ConfigurationService::setStringValue(const std::string &key,
										  const std::string &value) {
	this->setStringValue(GENERAL_CONF_FIELD, key, value);
}
void ConfigurationService::saveConfiguration() {
	this->iniFile->write(this->iniStructure);
}
