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

class translation_service {
public:
  translation_service();
  /*
    Current Language
    This allows the user to change the current language.
  */
  string current_language = DEFAULT_LANGUAGE;
  map<string, map<string, string, less<>>, less<>> translations = {};
  string get_translation_by_key(const char *key);
  string get_translation_by_key(const char *key, const char *c_language);
  string get_bare_key_by_language_reference(string language_key);
};
#endif
