
#include "raylib.h"
#include <functional>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include "ini.h"

#ifndef RPGPP_HOTKEYSERVICE_H
#define RPGPP_HOTKEYSERVICE_H

class Editor;

class HotkeyService {
	private:
		std::multimap<std::string, std::function<void()>> hotkeysCb;
		std::unordered_map<std::string, std::vector<KeyboardKey>> hotkeyMap;
		std::unordered_map<std::string, bool> activatedHotkey;
		void write(const std::string& keyId, const std::string& keyStr);
	public:
		HotkeyService();
		void registerHotkeyCallback(const std::string &keyId, std::function<void()> cb);
		void unregisterHotkeyCallback(const std::string &keyId);
		void addHotkey(const std::string &keyId, std::vector<KeyboardKey> keys);
		void removeHotkey(const std::string &keyId);
		std::map<std::string, std::string> serialize();
		void deserialize(const std::map<std::string, std::string> &serialized);
		void deserialize(mINI::INIMap<std::basic_string<char>> iniSerialized);
		void fire();
};

#endif
