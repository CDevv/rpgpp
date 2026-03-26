#ifndef _RPGPP_TRANSLATION_SERVICE_H
#define _RPGPP_TRANSLATION_SERVICE_H

#include <TGUI/String.hpp>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

class Editor;

constexpr auto TRANSLATION_FILE_LOCATION = "translations";
constexpr auto DEFAULT_LANGUAGE = "en_us";

// A custom string with functionality to replace placeholders in translation
// string. Can be used directly as a normal `std::string` or `tgui::String` when
// passing as arguments, or converted into std/tgui string with `static_cast`.
// Placeholders are defined with two curly brackets (like `{{test}}`)
// and can be replaced using `.replace("test", "value")`
class TranslatedString {
  private:
	std::string value;

  public:
	TranslatedString(const std::string &value) : value(value) {}
	TranslatedString replace(const std::string &k, const std::string &v) {
		std::string finder = "{{" + k + "}}";
		size_t pos = value.find(finder);
		if (pos != std::string::npos) {
			value.replace(pos, finder.length(), v);
		}
		return TranslatedString(value);
	}
	operator const std::string &() const { return value; }
	operator const tgui::String() const { return tgui::String{value}; }
};

class TranslationService {
  public:
	using ListenerID = size_t;
	using Callback = std::function<bool(TranslationService &ts, ListenerID id,
										bool checkingAlive)>;
	TranslationService(Editor *editor_ptr);
	TranslatedString getKey(const std::string &key);
	void setLanguage(const std::string &language);
	std::string getCurrentLanguage() const { return current_language; }
	std::string getLanguageIdentifierByKey(const std::string &language_key);
	ListenerID addListener(Callback cb);
	void removeListener(ListenerID id);
	void purgeDeadListeners();
	std::map<std::string, std::string> langKeyToName = {};

  private:
	std::map<std::string, std::map<std::string, std::string, std::less<>>,
			 std::less<>>
		translations = {};
	std::map<std::string, const filesystem::directory_entry> translationFiles = {};
	std::string current_language = DEFAULT_LANGUAGE;
	std::unordered_map<ListenerID, Callback> listeners;
	ListenerID lastID = 1;
	void notify();
	void loadTranslation(const std::string &langKey);
	void unloadTranslation(const std::string &langKey);
};
#endif
