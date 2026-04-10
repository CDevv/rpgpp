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

static void flattenJson(const json &j, std::map<std::string, std::string, std::less<>> &out,
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

void TranslationService::loadTranslation(const std::string &langKey) {
	if (translations.find(langKey) != translations.end()) return;
	const auto directory_entry = translationFiles.at(langKey);
	ifstream file(directory_entry.path());
	json parsed = json::parse(file);

	std::map<std::string, std::string, std::less<>> translated;
	flattenJson(parsed, translated);
	this->translations.try_emplace(langKey, std::move(translated));
}

void TranslationService::unloadTranslation(const std::string &langKey) {
	if (langKey == DEFAULT_LANGUAGE) return;
	this->translations.erase(langKey);
}

TranslationService::TranslationService(Editor *editor_ptr) {
	auto languageInOptions = editor_ptr->getConfiguration().getStringValue("language");
	for (auto const &directory_entry :
		 filesystem::directory_iterator(editor_ptr->getFs().getResourcePath(TRANSLATION_FILE_LOCATION))) {
		// add the translation to the translations map.
		if (directory_entry.path().extension() != ".json") continue;
		const std::string langKey = directory_entry.path().stem().string();
		auto [it, inserted] = translationFiles.try_emplace(langKey, directory_entry);

		const auto &entry = it->second;
		ifstream file(entry.path());
		json parsedEntry = json::parse(file);
		langKeyToName.try_emplace(langKey, parsedEntry.at("language").get<std::string>());

		if (langKey == DEFAULT_LANGUAGE || langKey == languageInOptions) {
			loadTranslation(langKey);
		}
	}
	if (translationFiles.find(languageInOptions) != translationFiles.end()) {
		current_language = languageInOptions;
	} else {
		fprintf(stderr, "WARNING: language '%s' not found, falling back to default.\n", languageInOptions.c_str());
		current_language = DEFAULT_LANGUAGE;
	}
}

void TranslationService::setLanguage(const std::string &language) {
	if (language != current_language) {
		unloadTranslation(current_language);
	}
	current_language = language;
	loadTranslation(language);

	for (auto &[id, cb] : listeners) {
		cb(*this, id, false);
	}
}

TranslationService::ListenerID TranslationService::addListener(Callback cb) {
	ListenerID id = lastID++;
	listeners[id] = std::move(cb);
	purgeDeadListeners();
	return id;
}

void TranslationService::removeListener(ListenerID id) {
	if (listeners.find(id) != listeners.end()) {
		listeners.erase(id);
	}
}

void TranslationService::purgeDeadListeners() {
	vector<ListenerID> toBePurged;
	for (auto &[id, cb] : listeners) {
		if (!cb(*this, id, true)) {
			toBePurged.push_back(id);
		}
	}
	for (auto id : toBePurged) {
		removeListener(id);
	}
}

// @notice Use of getKey is not recommended unless you guarantee that the widget
// using the provided translation can update itself when translation changes.
// Otherwise, please use `bindTranslation()` in `bindTranslation.hpp`
TranslatedString TranslationService::getKey(const std::string &key) {
	if (translations.find(getCurrentLanguage()) != translations.end()) {
		const auto &gotten_translations = translations[current_language];
		if (gotten_translations.find(key) != gotten_translations.end()) {
			TranslatedString s = TranslatedString{gotten_translations.at(key)};
			return s;
		}
		TranslatedString s = TranslatedString{translations[DEFAULT_LANGUAGE][key]};
		printf("TRANSLATION WARNING: %s key doesn't exist for language %s\n", key.c_str(), current_language.c_str());
		return s;
	} else {
		throw std::out_of_range("translation doesn't exist in translations.");
	}
}

// NOTE: the whole point of this function, is to retrieve the raw key of a
// language file like "en_us", from the "language" key in the JSON.
std::string TranslationService::getLanguageIdentifierByKey(const std::string &language_key) {
	return std::find_if(langKeyToName.begin(), langKeyToName.end(),
						[&](std::pair<std::string, std::string> entry) { return entry.second == language_key; })
		->first;
}
