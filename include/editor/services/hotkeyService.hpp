
#include "ini.h"
#include "raylib.h"
#include <functional>
#include <nlohmann/json.hpp>
#include <unordered_map>

#ifndef RPGPP_HOTKEYSERVICE_H
#define RPGPP_HOTKEYSERVICE_H

class Editor;
struct Hotkey {
	bool ctrl;
	bool shift;
	bool alt;
	bool super;
	KeyboardKey key;
};
using HotkeyMap = std::unordered_map<std::string, Hotkey>;
class HotkeyService {
  private:
	std::multimap<std::string, std::function<void()>> hotkeysCb;
	void write(const std::string &keyId, const std::string &keyStr);
	HotkeyMap hotkeyMap;

  public:
	HotkeyService();
	void registerHotkeyCallback(const std::string &keyId,
								std::function<void()> cb);
	void unregisterHotkeyCallback(const std::string &keyId);
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
