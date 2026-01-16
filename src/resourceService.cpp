#include "resourceService.hpp"
#include <cstdio>
#include <raylib.h>

ResourceService::ResourceService() {
  addTextureFromFile("resources/logo.png");
  addTextureFromFile("resources/close.png");
  addTextureFromFile("resources/cross.png");
  addTextureFromFile("resources/dialog.png");
  addTextureFromFile("resources/figurine.png");
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

void ResourceService::unload() const {
  for (const auto &[name, texture] : textures) {
    UnloadTexture(texture);
  }
}

Texture2D ResourceService::getTexture(const std::string &id) {
  return textures[id];
}
