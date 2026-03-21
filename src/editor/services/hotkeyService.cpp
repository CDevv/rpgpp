#include "services/hotkeyService.hpp"
#include "raylib.h"

HotkeyService::HotkeyService() {}

void HotkeyService::registerHotkeyCallback(std::string keyId, std::function<void()> cb) {
	activatedHotkey[keyId] = false;
	hotkeysCb.insert({keyId, cb});
}

void HotkeyService::unregisterHotkeyCallback(std::string keyId) {
	hotkeysCb.erase(keyId);
}

void HotkeyService::addHotkey(std::string keyId, std::vector<KeyboardKey> keys) {
	activatedHotkey[keyId] = false;
	hotkeyMap[keyId] = keys;
}

void HotkeyService::removeHotkey(std::string keyId) {
	hotkeyMap.erase(keyId);
}

void HotkeyService::fire() {
	for (auto &[keyId, keys] : hotkeyMap) {
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
