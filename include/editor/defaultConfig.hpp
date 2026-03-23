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
	 {{"close_tab", h(Hotkey{}.withCtrl().withKey(KEY_W))},
	  {"new_project", h(Hotkey{}.withCtrl().withKey(KEY_N))},
	  {"open_project", h(Hotkey{}.withCtrl().withKey(KEY_O))},
	  {"redo", h(Hotkey{}.withCtrl().withKey(KEY_Y))},
	  {"save_file", h(Hotkey{}.withCtrl().withKey(KEY_S))},
	  {"toggle_debug", h(Hotkey{}.withKey(KEY_F3))},
	  {"undo", h(Hotkey{}.withCtrl().withKey(KEY_Z))},
	  {"room_tool.mouse", h(Hotkey{}.withKey(KEY_ONE))},
	  {"room_tool.pen", h(Hotkey{}.withKey(KEY_TWO))},
	  {"room_tool.eraser", h(Hotkey{}.withKey(KEY_THREE))},
	  {"room_tool.edit", h(Hotkey{}.withKey(KEY_FOUR))},
	  {"room_tool.set_spoint", h(Hotkey{}.withKey(KEY_FIVE))},
	  {"room_tool.toggle_bm", h(Hotkey{}.withKey(KEY_SIX))}}},
};
