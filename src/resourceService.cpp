#include "resourceService.hpp"

#include <raylib.h>

#include <cstdio>

#include "game.hpp"

ResourceService::ResourceService() {
	addTextureFromFile("resources/logo.png");
	addTextureFromFile("resources/close.png");
	addTextureFromFile("resources/cross.png");
	addTextureFromFile("resources/dialog.png");
	addTextureFromFile("resources/figurine.png");

	init();
}

void ResourceService::init() {
	unload();

	/*
	if (Game::isUsingBin()) {
		for (auto &[id, fontBin] : Game::getBin().fonts) {
			Font font = LoadFontFromMemory(fontBin.ext.c_str(), fontBin.data.data(), fontBin.dataSize, 13, nullptr, 0);
			addFont(id, font);
		}
	} else {
		auto dirList = LoadDirectoryFiles("fonts/");
		for (int i = 0; i < dirList.count; i++) {
			auto path = dirList.paths[i];
			addFontFromFile(path, 13);
		}
	}
		*/

	auto dirList = LoadDirectoryFiles("fonts/");
	for (int i = 0; i < dirList.count; i++) {
		auto path = dirList.paths[i];
		addFontFromFile(path, 13);
	}
}

ResourceService::~ResourceService() = default;

void ResourceService::addTexture(const std::string &id, Texture2D texture) {
	printf("%s was added \n", id.c_str());
	textures[id] = texture;
}

void ResourceService::addTextureFromFile(const std::string &filePath) {
	Texture2D text = LoadTexture(filePath.c_str());
	std::string newId = GetFileNameWithoutExt(filePath.c_str());
	addTexture(newId, text);
}

Texture2D ResourceService::getTexture(const std::string &id) { return textures[id]; }

void ResourceService::addFont(const std::string &id, Font font) { fonts[id] = font; }

void ResourceService::addFontFromFile(const std::string &filePath, int fontSize) {
	Font font = LoadFontEx(filePath.c_str(), fontSize, nullptr, 250);
	addFont(GetFileNameWithoutExt(filePath.c_str()), font);
}

Font ResourceService::getFont(const std::string &id) { return fonts.at(id); }

void ResourceService::unload() const {
	for (const auto &[name, texture] : textures) {
		if (IsTextureValid(texture)) {
			UnloadTexture(texture);
		}
	}

	for (const auto &[id, font] : fonts) {
		if (IsFontValid(font)) {
			UnloadFont(font);
		}
	}
}
