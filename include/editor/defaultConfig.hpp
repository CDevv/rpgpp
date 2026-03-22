#include "editor.hpp"
#include "raylib.h"
#include "services/hotkeyService.hpp"
#include <map>
#include <string>
#include <utility>
#include <vector>

// key-value
using ConfigEntry = std::map<std::string, std::string>;
// [field]
using Config = std::map<std::string, ConfigEntry>;

const std::string h(Hotkey hk) {
	return std::to_string(HotkeyService::pack(hk));
}

const Config BASE_CONFIG = {
	{"rpgpp",
	 {
		 {"language", "en_us"},
		 {"theme", "Dark"},
	 }},
	{"hotkeys",
	 {{"close_tab", h({.ctrl = true, .key = KEY_W})},
	  {"new_project", h({.ctrl = true, .key = KEY_N})},
	  {"open_project", h({.ctrl = true, .key = KEY_O})},
	  {"redo", h({.ctrl = true, .key = KEY_Y})},
	  {"save_file", h({.ctrl = true, .key = KEY_S})},
	  {"toggle_debug", h({.key = KEY_F3})},
	  {"undo", h({.ctrl = true, .key = KEY_Z})},
	  {"room_tool.mouse", h({.key = KEY_ONE})},
	  {"room_tool.pen", h({.key = KEY_TWO})},
	  {"room_tool.eraser", h({.key = KEY_THREE})},
	  {"room_tool.edit", h({.key = KEY_FOUR})},
	  {"room_tool.set_spoint", h({.key = KEY_FIVE})},
	  {"room_tool.toggle_bm", h({.key = KEY_SIX})}}},
};
