#include "services/translationService.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>

#include "editor.hpp"

using json = nlohmann::json;

TranslationService::TranslationService() {
	for (auto const &directory_entry :
		 filesystem::directory_iterator(Editor::instance->getFs().getResourcePath(TRANSLATION_FILE_LOCATION))) {
		// add the translation to the translations map.
		this->translations.try_emplace(
			directory_entry.path().stem().string(),
			json::parse(ifstream(directory_entry.path())));
	}
	this->current_language = Editor::instance->getConfiguration().getStringValue("language");
}

TranslatedString getKeyWrapper(TranslationService *tr, const std::string &c_language,
			  const std::string &key) {
	if (tr->translations.find(tr->current_language) !=
		tr->translations.end()) {
		map<string, string, less<>> gotten_translations =
			tr->translations[c_language];
		if (gotten_translations.find(key) !=
			gotten_translations.end()) {
			TranslatedString s = TranslatedString{gotten_translations[key]};
			return s;
		}
		TranslatedString s = TranslatedString{tr->translations[DEFAULT_LANGUAGE][key]};
		return s;
	} else {
		throw std::out_of_range(
			"translation doesn't exist in translations.");
	}
}

TranslatedString TranslationService::getKey(const std::string &key) {
	return getKeyWrapper(this, current_language, key);
}

TranslatedString TranslationService::getKey(const std::string &key,
					   const std::string &c_language) {
	return getKeyWrapper(this, c_language, key);
}

// NOTE: the whole point of this function, is to retrieve the raw key of a language file like "en_us", from the "language" key in the JSON.
std::string TranslationService::getLanguageIdentifierByKey(const std::string& language_key) {
	return std::find_if(this->translations.begin(), this->translations.end(),
	                    [&](std::pair<std::string, std::map<std::string, std::string, std::less<>>> entry) {
		                    return entry.second["language"] == language_key;
	                    })->first;
}
