#include "services/configurationService.hpp"
#include "defaultConfig.hpp"
#include "ini.h"
#include "raylib.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

void ConfigurationService::regenerate() {
	assert(this->iniFile && "iniFile is not initialized");
	for (auto &[field, kv] : BASE_CONFIG) {
		for (auto &[k, v] : kv) {
			if (!this->iniStructure.has(field)) {
				this->iniStructure.set(field, mINI::INIMap<std::string>());
			}
			if (this->iniStructure[field].has(k))
				continue;
			this->iniStructure[field].set(k, v);
		}
	}
	this->iniFile->write(this->iniStructure);
}

ConfigurationService::ConfigurationService() {
	std::filesystem::path path = GetWorkingDirectory();
	path /= RPGPP_CONFIG_FILE;
	bool notExist = !std::filesystem::exists(path);
	if (notExist) {
		std::ofstream output(path);
		output.close();
	}
	this->iniFile = std::make_unique<mINI::INIFile>(path.string());
	this->iniFile->read(this->iniStructure);
	this->regenerate();
};

mINI::INIMap<std::basic_string<char>>
ConfigurationService::getField(const std::string &field) {
	return this->iniStructure[field];
}

std::string ConfigurationService::getStringValue(const std::string &field,
												 const std::string &key) {
	return this->iniStructure[field][key];
}

std::string ConfigurationService::getStringValue(const std::string &key) {
	return this->getStringValue(GENERAL_CONF_FIELD, key);
}

void ConfigurationService::setStringValue(const std::string &field,
										  const std::string &key,
										  const std::string &value) {
	this->iniStructure[field].set(key, value);
}

void ConfigurationService::setStringValue(const std::string &key,
										  const std::string &value) {
	this->setStringValue(GENERAL_CONF_FIELD, key, value);
}
void ConfigurationService::saveConfiguration() {
	this->iniFile->write(this->iniStructure);
}
