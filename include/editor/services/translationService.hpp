#ifndef _RPGPP_TRANSLATION_SERVICE_H
#define _RPGPP_TRANSLATION_SERVICE_H

#include <functional>
#include <map>
#include <string>
#include <TGUI/String.hpp>
using namespace std;

constexpr auto TRANSLATION_FILE_LOCATION = "translations";
constexpr auto DEFAULT_LANGUAGE = "en_us";

// A custom string with functionality to replace placeholders in translation string.
// Can be used directly as a normal `std::string` or `tgui::String` when passing as arguments, or converted into std/tgui string with `static_cast`.
// Placeholders are defined with two curly brackets (like `{{test}}`)
// and can be replaced using `.replace("test", "value")`
class TranslatedString {
    private:
        std::string value;
    public:
        TranslatedString(const std::string& value) : value(value) {}
        TranslatedString replace(const std::string& k, const std::string& v) {
            std::string finder = "{{" + k + "}}";
            size_t pos = value.find(finder);
            if (pos != std::string::npos) {
                value.replace(pos, finder.length(), v);
            }
            return TranslatedString(value);
        }
        operator const std::string&() const { return value; }
        operator const tgui::String() const { return tgui::String{value}; }
};

class TranslationService {
public:
  TranslationService();
  std::string current_language = DEFAULT_LANGUAGE;
  std::map<std::string, std::map<std::string, std::string, std::less<>>,
           std::less<>>
      translations = {};
  TranslatedString getKey(const std::string &key);
  TranslatedString getKey(const std::string &key, const std::string &c_language);
  std::string getLanguageIdentifierByKey(const std::string& language_key);
};
#endif
