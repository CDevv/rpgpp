#include "translationService.hpp"
#include "nlohmann/json.hpp" // YES IT USED!! DO NOT REMOVE
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;

translation_service::translation_service() {
  for (auto const &directory_entry :
       filesystem::directory_iterator(TRANSLATION_FILE_LOCATION)) {
    // add the translation to the translations map.
    this->translations.try_emplace(
        directory_entry.path().stem().string(),
        nlohmann::json::parse(ifstream(directory_entry.path())));
  }
}

// Wrapper for the translation system get_translation_by_key, because C++ hates
// me referencing another function in inheritence.
string get_translation_by_key_wrapper(translation_service *tr,
                                           const char *c_language,
                                           const char *key) {
  if (tr->translations.find(tr->current_language) != tr->translations.end()) {
    map<string, string, less<>> gotten_translations =
        tr->translations[c_language];
    if (gotten_translations.find(key) != gotten_translations.end()) {
      return gotten_translations[key];
    } else {
      // pls shut up linter waaaaaaaaaaahhhhh *crying* :<
      return get_translation_by_key_wrapper(tr, DEFAULT_LANGUAGE, key);
    }
  } else {
    throw new out_of_range("translation doesn't exist in translations.");
  }
}

string translation_service::get_translation_by_key(const char *key) {
  return get_translation_by_key_wrapper(this, current_language.c_str(), key);
}

string translation_service::get_translation_by_key(const char *key,
                                                   const char *c_language) {
  return get_translation_by_key_wrapper(this, c_language, key);
}
