#include "services/hotkeyService.hpp"
#include "raylib.h"
#include <iostream>
#include <random>
#include <string>
HotkeyService::HotkeyService() {}

// It's not true UUID, but it will work in this case
// https://stackoverflow.com/a/58467162
std::string get_uuid() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

    std::string res;
    for (int i = 0; i < 16; i++) {
        if (dash[i]) res += "-";
        res += v[dist(rng)];
        res += v[dist(rng)];
    }
    return res;
}

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
	return Hotkey{
		(bool)((packed >> 0) & 1),
		(bool)((packed >> 1) & 1),
		(bool)((packed >> 2) & 1),
		(bool)((packed >> 3) & 1),
		static_cast<KeyboardKey>(packed >> 4)
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

std::string HotkeyService::registerHotkeyCallback(const std::string &keyId,
										   std::function<void()> cb) {
	std::string uniqueHkCbId = get_uuid();
	hotkeysCb[uniqueHkCbId] = {keyId, cb};
	return uniqueHkCbId;
}

void HotkeyService::unregisterHotkeyCallback(const std::string &uniqueHkCbId) {
	hotkeysCb.erase(uniqueHkCbId);
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
			for (auto [_, data] : hotkeysCb) {
				if (data.first == keyId) {
					data.second();
				}
			}
		}
	}
}
