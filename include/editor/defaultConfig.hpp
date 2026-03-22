#include "raylib.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

// key-value
using ConfigEntry = std::map<std::string, std::string>;
// [field]
using Config = std::map<std::string, ConfigEntry>;

const Config BASE_CONFIG = {
    {"rpgpp", {
        {"language", "en_us"},
        {"theme",    "Dark"},
    }},
    {"hotkeys", {
        {"close_tab",     "341,87,"},
        {"new_project",   "341,78,"},
        {"open_project",  "341,79,"},
        {"redo",          "341,89,"},
        {"save_file",     "341,83,"},
        {"toggle_debug",  "292,"},
        {"undo",          "341,90,"},
    }},
};
