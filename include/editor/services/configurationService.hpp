#ifndef RPGPP_CONFIGURATIONSERVICE_H
#define RPGPP_CONFIGURATIONSERVICE_H

#include "ini.h"

constexpr auto GENERAL_CONF_FIELD = "rpgpp";
#define RPGPP_CONFIG_FILE "rpgpp.ini"

class ConfigurationService {
	private:
		std::unique_ptr<mINI::INIFile> iniFile;
		mINI::INIStructure iniStructure;
		void regenerate();
  	public:
		ConfigurationService();
		mINI::INIMap<std::basic_string<char>> getField(const std::string &field);
		std::string getStringValue(const std::string &key);
		std::string getStringValue(const std::string& field, const std::string &key);
		void setStringValue(const std::string &key, const std::string &value);
		void setStringValue(const std::string& field, const std::string &key, const std::string &value);
		void saveConfiguration();
};

#endif // RPGPP_CONFIGURATIONSERVICE_H
