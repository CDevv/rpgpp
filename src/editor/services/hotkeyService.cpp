#include "services/hotkeyService.hpp"
#include "raylib.h"
#include <iostream>
#include <string>

HotkeyService::HotkeyService() {}

const int HotkeyService::pack(Hotkey hk) {
	int k = 0;
	k |= hk.ctrl * (1 << 0);
	k |= hk.shift * (1 << 1);
	k |= hk.alt * (1 << 2);
	k |= hk.super * (1 << 3);
	k |= hk.key << 4;
	return k;
}

const Hotkey HotkeyService::unpack(int packed) {
	return {
		.ctrl = (bool)((packed >> 0) & 1),
		.shift = (bool)((packed >> 1) & 1),
		.alt = (bool)((packed >> 2) & 1),
		.super = (bool)((packed >> 3) & 1),
		.key = static_cast<KeyboardKey>(packed >> 4),
	};
}

std::map<std::string, std::string> HotkeyService::serialize() {
	std::map<std::string, std::string> serialized{};
	for (auto &[keyId, keys] : hotkeyMap) {
		int k = pack(keys);
		std::string keyStr = std::to_string(k);
		serialized[keyId] = keyStr;
	}
	return serialized;
}

void HotkeyService::write(const std::string &keyId, const std::string &keyStr) {
	int k = stoi(keyStr);
	hotkeyMap[keyId] = unpack(k);
}

void HotkeyService::deserialize(
	const std::map<std::string, std::string> &serialized) {
	hotkeyMap.clear();
	for (auto &[keyId, keyStr] : serialized) {
		write(keyId, keyStr);
	}
}

void HotkeyService::deserialize(
	mINI::INIMap<std::basic_string<char>> iniSerialized) {
	hotkeyMap.clear();
	for (auto &[keyId, keyStr] : iniSerialized) {
		write(keyId, keyStr);
	}
}

void HotkeyService::registerHotkeyCallback(const std::string &keyId,
										   std::function<void()> cb) {
	hotkeysCb.insert({keyId, cb});
}

void HotkeyService::unregisterHotkeyCallback(const std::string &keyId) {
	hotkeysCb.erase(keyId);
}

void HotkeyService::addHotkey(const std::string &keyId, const Hotkey &keys) {
	hotkeyMap[keyId] = keys;
}

void HotkeyService::removeHotkey(const std::string &keyId) {
	hotkeyMap.erase(keyId);
}

const HotkeyMap HotkeyService::listHotkeys() { return this->hotkeyMap; }

void HotkeyService::fire() {
	for (auto &[keyId, keys] : hotkeyMap) {
		if ((keys.ctrl ^ IsKeyDown(KEY_LEFT_CONTROL)))
			continue;
		if ((keys.shift ^ IsKeyDown(KEY_LEFT_SHIFT)))
			continue;
		if ((keys.alt ^ IsKeyDown(KEY_LEFT_ALT)))
			continue;
		if ((keys.super ^ IsKeyDown(KEY_LEFT_SUPER)))
			continue;
		if (IsKeyDown(keys.key)) {
			auto range = hotkeysCb.equal_range(keyId);
			for (auto it = range.first; it != range.second; ++it) {
				it->second();
			}
		}
	}
}
