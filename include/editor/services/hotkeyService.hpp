
#include <functional>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "ini.h"
#include "raylib.h"

#ifndef RPGPP_HOTKEYSERVICE_H
#define RPGPP_HOTKEYSERVICE_H

class Editor;
struct Hotkey {
	bool ctrl = false;
	bool shift = false;
	bool alt = false;
	bool super = false;
	KeyboardKey key = KEY_NULL;

	Hotkey &withCtrl(bool is = true) {
		ctrl = is;
		return *this;
	}
	Hotkey &withShift(bool is = true) {
		shift = is;
		return *this;
	}
	Hotkey &withAlt(bool is = true) {
		alt = is;
		return *this;
	}
	Hotkey &withSuper(bool is = true) {
		super = is;
		return *this;
	}
	Hotkey &withKey(KeyboardKey k) {
		key = k;
		return *this;
	}
};
using HotkeyMap = std::unordered_map<std::string, Hotkey>;
class HotkeyService {
private:
	std::map<std::string, std::pair<std::string, std::function<void()>>> hotkeysCb;
	void write(const std::string &keyId, const std::string &keyStr);
	HotkeyMap hotkeyMap;

public:
	HotkeyService();
	std::string registerHotkeyCallback(const std::string &keyId, std::function<void()> cb);
	void unregisterHotkeyCallback(const std::string &uniqueHkCbId);
	void addHotkey(const std::string &keyId, const Hotkey &keys);
	void removeHotkey(const std::string &keyId);
	const HotkeyMap listHotkeys();
	std::map<std::string, std::string> serialize();
	void deserialize(const std::map<std::string, std::string> &serialized);
	void deserialize(mINI::INIMap<std::basic_string<char>> iniSerialized);
	void fire();
	static const int pack(Hotkey hk);
	static const Hotkey unpack(int packed);
};

#endif
