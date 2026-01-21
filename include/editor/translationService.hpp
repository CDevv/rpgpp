#ifndef _RPGPP_TRANSLATION_SERVICE_H
#define _RPGPP_TRANSLATION_SERVICE_H

#include <functional>
#include <map>
#include <string>
using namespace std;

constexpr const char *TRANSLATION_FILE_LOCATION = "resources/translations";
constexpr const char *DEFAULT_LANGUAGE = "en_us";

/*
    How this system works:
        1. Add a file to resources/translations/<language_key>.json
        2. It must contain all the specified keys in any other file, otherwise
   the engine will not accept it.
        3. and that's all!
*/

class TranslationService {
public:
  TranslationService();
  /*
    Current Language
    This allows the user to change the current language.
  */
  std::string current_language = "en_us";
  std::map<std::string, std::map<std::string, std::string, std::less<>>,
           std::less<>>
      translations = {};
  std::string getKey(const std::string &key);
  std::string getKey(const std::string &key, const std::string &c_language);
  std::string getLanguageIdentifierByKey(const std::string& language_key);
};
#endif
