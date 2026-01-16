#include "translationService.hpp"
#include "nlohmann/json.hpp" // YES IT USED!! DO NOT REMOVE
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>

translation_service::translation_service() {
  for (auto const &directory_entry :
       std::filesystem::directory_iterator(TRANSLATION_FILE_LOCATION)) {
    // add the translation to the translations map.
    this->translations.try_emplace(
        directory_entry.path().stem().string(),
        nlohmann::json::parse(std::ifstream(directory_entry.path())));
  }
}

// Wrapper for the translation system get_translation_by_key, because C++ hates
// me referencing another function in inheritence.
std::string get_translation_by_key_wrapper(translation_service *tr,
                                           const char *c_language,
                                           const char *key) {
  if (tr->translations.find(tr->current_language) != tr->translations.end()) {
    std::map<std::string, std::string, std::less<>> gotten_translations =
        tr->translations[c_language];
    if (gotten_translations.find(key) != gotten_translations.end()) {
      return gotten_translations[key];
    } else {
      // pls shut up linter waaaaaaaaaaahhhhh *crying* :<
      throw new std::out_of_range("key not found in translations.");
    }
  } else {
    throw new std::out_of_range("translation doesn't exist in translations.");
  }
}

std::string translation_service::get_translation_by_key(const char *key) {
  return get_translation_by_key_wrapper(this, current_language.c_str(), key);
}

std::string
translation_service::get_translation_by_key(const char *key,
                                            const char *c_language) {
  return get_translation_by_key_wrapper(this, c_language, key);
}
