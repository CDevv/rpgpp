#include "services/translationService.hpp"
#include <cstdio>
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

static void flattenJson(const json &j,
						std::map<std::string, std::string, std::less<>> &out,
						const std::string &prefix = "") {
	for (auto it = j.begin(); it != j.end(); ++it) {
		std::string key = prefix.empty() ? it.key() : prefix + "." + it.key();
		if (it->is_object()) {
			flattenJson(*it, out, key);
		} else if (it->is_string()) {
			out[key] = it->get<std::string>();
		} else {
			out[key] = it->dump();
		}
	}
}

TranslationService::TranslationService(Editor *editor_ptr) {
	for (auto const &directory_entry : filesystem::directory_iterator(
			 editor_ptr->getFs().getResourcePath(TRANSLATION_FILE_LOCATION))) {
		// add the translation to the translations map.
		if (directory_entry.path().extension() != ".json")
			continue;

		ifstream file(directory_entry.path());
		json parsed = json::parse(file);

		std::map<std::string, std::string, std::less<>> translated;
		flattenJson(parsed, translated);
		this->translations.try_emplace(directory_entry.path().stem().string(),
									   std::move(translated));
	}
	this->current_language =
		editor_ptr->getConfiguration().getStringValue("language");
}

TranslatedString getKeyWrapper(TranslationService *tr,
							   const std::string &c_language,
							   const std::string &key) {
	if (tr->translations.find(tr->getCurrentLanguage()) != tr->translations.end()) {
		map<string, string, less<>> gotten_translations =
			tr->translations[c_language];
		if (gotten_translations.find(key) != gotten_translations.end()) {
			TranslatedString s = TranslatedString{gotten_translations[key]};
			return s;
		}
		TranslatedString s =
			TranslatedString{tr->translations[DEFAULT_LANGUAGE][key]};
		printf("TRANSLATION WARNING: %s key doesn't exist for language %s\n",
			   key.c_str(), c_language.c_str());
		return s;
	} else {
		throw std::out_of_range("translation doesn't exist in translations.");
	}
}

void TranslationService::setLanguage(const std::string &language) {
	current_language = language;

	for (auto& [id, cb] : listeners) {
		cb(id);
	}
}

TranslationService::ListenerID TranslationService::addListener(Callback cb) {
	ListenerID id = lastID++;
	listeners[id] = std::move(cb);
	std::cout << listeners.size() << "\n";
	return id;
}

void TranslationService::removeListener(ListenerID id) {
	if (listeners.find(id) != listeners.end()) {
		listeners.erase(id);
	}
}

// @notice Use of getKey is not recommended unless you guarantee that the widget using the provided
// translation can update itself when translation changes. Otherwise, please use `bindTranslation()` in `bindTranslation.hpp`
TranslatedString TranslationService::getKey(const std::string &key) {
	return getKeyWrapper(this, current_language, key);
}

// @notice Use of getKey is not recommended unless you guarantee that the widget using the provided
// translation can update itself when translation changes. Otherwise, please use `bindTranslation()` in `bindTranslation.hpp`
TranslatedString TranslationService::getKey(const std::string &key,
											const std::string &c_language) {
	return getKeyWrapper(this, c_language, key);
}

// NOTE: the whole point of this function, is to retrieve the raw key of a
// language file like "en_us", from the "language" key in the JSON.
std::string TranslationService::getLanguageIdentifierByKey(
	const std::string &language_key) {
	return std::find_if(
			   this->translations.begin(), this->translations.end(),
			   [&](std::pair<std::string,
							 std::map<std::string, std::string, std::less<>>>
					   entry) {
				   return entry.second["language"] == language_key;
			   })
		->first;
}
