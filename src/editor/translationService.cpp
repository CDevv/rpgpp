#include "translationService.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

TranslationService::TranslationService() {
	for (auto const &directory_entry :
		 filesystem::directory_iterator(TRANSLATION_FILE_LOCATION)) {
		// add the translation to the translations map.
		this->translations.try_emplace(
			directory_entry.path().stem().string(),
			json::parse(ifstream(directory_entry.path())));
	}
}

std::string getKeyWrapper(TranslationService *tr, const std::string &c_language,
			  const std::string &key) {
	if (tr->translations.find(tr->current_language) !=
		tr->translations.end()) {
		map<string, string, less<>> gotten_translations =
			tr->translations[c_language];
		if (gotten_translations.find(key) !=
			gotten_translations.end()) {
			return gotten_translations[key];
		} else {
			// pls shut up linter waaaaaaaaaaahhhhh *crying* :<
			throw std::out_of_range(
				"key not found in translations.");
		}
	} else {
		throw std::out_of_range(
			"translation doesn't exist in translations.");
	}
}

std::string TranslationService::getKey(const std::string &key) {
	return getKeyWrapper(this, current_language, key);
}

std::string TranslationService::getKey(const std::string &key,
					   const std::string &c_language) {
	return getKeyWrapper(this, c_language, key);
}
