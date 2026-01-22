#ifndef _RPGPP_TRANSLATION_SERVICE_H
#define _RPGPP_TRANSLATION_SERVICE_H

#include <functional>
#include <map>
#include <string>
using namespace std;

constexpr auto TRANSLATION_FILE_LOCATION = "resources/translations";
constexpr auto DEFAULT_LANGUAGE = "en_us";

class TranslationService {
public:
  TranslationService();
  std::string current_language = DEFAULT_LANGUAGE;
  std::map<std::string, std::map<std::string, std::string, std::less<>>,
           std::less<>>
      translations = {};
  std::string getKey(const std::string &key);
  std::string getKey(const std::string &key, const std::string &c_language);
  std::string getLanguageIdentifierByKey(const std::string& language_key);
};
#endif
