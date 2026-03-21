#include "raylib.h"
#include <string>
#include <utility>
#include <vector>
const std::vector<std::pair<const std::string, const std::vector<KeyboardKey>>> DEFAULT_HOTKEYS = {
	{"toggle_debug", std::vector<KeyboardKey>{KEY_F3}},
	{"close_tab", std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_W}},
	{"open_project", std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_O}},
	{"save_file", std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_S}},
	{"undo", std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_Z}},
	{"redo", std::vector<KeyboardKey>{KEY_LEFT_CONTROL, KEY_Y}},
};
