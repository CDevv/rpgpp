#include "services/hotkeyService.hpp"
#include "raylib.h"

HotkeyService::HotkeyService() {}

std::map<std::string, std::string> HotkeyService::serialize() {
	std::map<std::string, std::string> serialized{};
	for (auto &[keyId, keys]: hotkeyMap) {
		std::string keyStr{};
		for (KeyboardKey k : keys) {
			keyStr += std::to_string(k) + ",";
		}
		serialized[keyId] = keyStr;
	}
	return serialized;
}

void HotkeyService::write(const std::string& keyId, const std::string& keyStr) {
	std::vector<KeyboardKey> keys{};
	std::stringstream ss(keyStr);
	std::string token;
	while (std::getline(ss, token, ',')) {
		if (!token.empty())
			keys.push_back(static_cast<KeyboardKey>(std::stoi(token)));
	}
	hotkeyMap[keyId] = keys;
}

void HotkeyService::deserialize(const std::map<std::string, std::string> &serialized) {
	hotkeyMap.clear();
	for (auto &[keyId, keyStr]: serialized) {
		write(keyId, keyStr);
	}
}

void HotkeyService::deserialize(mINI::INIMap<std::basic_string<char>> iniSerialized) {
	hotkeyMap.clear();
	for (auto &[keyId, keyStr]: iniSerialized) {
		write(keyId, keyStr);
	}
}

void HotkeyService::registerHotkeyCallback(const std::string& keyId, std::function<void()> cb) {
	activatedHotkey[keyId] = false;
	hotkeysCb.insert({keyId, cb});
}

void HotkeyService::unregisterHotkeyCallback(const std::string& keyId) {
	hotkeysCb.erase(keyId);
	if (hotkeyMap.find(keyId) == hotkeyMap.end())
        activatedHotkey.erase(keyId);
}

void HotkeyService::addHotkey(const std::string& keyId, std::vector<KeyboardKey> keys) {
	activatedHotkey[keyId] = false;
	hotkeyMap[keyId] = keys;
}

void HotkeyService::removeHotkey(const std::string& keyId) {
	hotkeyMap.erase(keyId);
	if (hotkeysCb.find(keyId) == hotkeysCb.end())
        activatedHotkey.erase(keyId);
}

void HotkeyService::fire() {
	for (auto &[keyId, keys] : hotkeyMap) {
		if (keys.empty()) continue;
		bool allDown = true;
		for (KeyboardKey k : keys) {
			if (!IsKeyDown(k)) {
				allDown = false;
				activatedHotkey[keyId] = false;
				break;
			}
		}
		if (allDown && !activatedHotkey[keyId]) {
			auto range = hotkeysCb.equal_range(keyId);
			for (auto it = range.first; it != range.second; ++it) {
				it->second();
			}
			activatedHotkey[keyId] = true;
		}
	}
}
