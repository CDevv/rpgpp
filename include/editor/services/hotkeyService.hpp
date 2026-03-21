
#include "raylib.h"
#include <functional>
#include <nlohmann/json.hpp>
#include <unordered_map>

#ifndef RPGPP_HOTKEYSERVICE_H
#define RPGPP_HOTKEYSERVICE_H

class Editor;

class HotkeyService {
	private:
		std::multimap<std::string, std::function<void()>> hotkeysCb;
		std::unordered_map<std::string, std::vector<KeyboardKey>> hotkeyMap;
		std::unordered_map<std::string, bool> activatedHotkey;
	public:
		HotkeyService();
		void registerHotkeyCallback(std::string keyId, std::function<void()> cb);
		void unregisterHotkeyCallback(std::string keyId);
		void addHotkey(std::string keyId, std::vector<KeyboardKey> keys);
		void removeHotkey(std::string keyId);
		void fire();
};

#endif
