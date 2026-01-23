#include "configurationService.h"

#include <iostream>

#include "raylib.h"


ConfigurationService::ConfigurationService() {
	if (std::filesystem::exists(RPGPP_CONFIG_FILE)) {
		this->iniFile = std::make_unique<mINI::INIFile>(RPGPP_CONFIG_FILE);
		this->iniFile->read(this->iniStructure);
	} else {
		throw std::runtime_error(GetWorkingDirectory());
	}
};

std::string ConfigurationService::getStringValue(const std::string &key) {
	if (this->iniStructure[GENERAL_CONF_FIELD].has(key))
		return this->iniStructure[GENERAL_CONF_FIELD][key];
	std::stringstream ss;
	ss << "configuration key doesn't exist" << key;
	throw std::runtime_error(ss.str());
}

void ConfigurationService::setStringValue(const std::string &key, const std::string &value) {
	this->iniStructure[GENERAL_CONF_FIELD].set(key, value);
}

void ConfigurationService::saveConfiguration() {
	this->iniFile->write(this->iniStructure);
}
