#ifndef _RPGPP_FILESYSTEMSERVICE_H
#define _RPGPP_FILESYSTEMSERVICE_H

#include "variant.hpp"
#include <array>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#define FILETYPE_MAX 9

enum class EngineFileType {
	FILE_TILESET,
	FILE_MAP,
	FILE_ACTOR,
	FILE_DIALOGUE,
	FILE_IMAGE,
	FILE_FONT,
	FILE_SOUND,
	FILE_MUSIC,
	FILE_PROP,
	FILE_SCRIPT
};

class FileSystemService {
  private:
	std::array<std::string, FILETYPE_MAX> typeNames;
	std::string editorBaseDir;

  public:
	FileSystemService();
	void unload();
	void promptOpenProject();
	std::string &getTypeName(EngineFileType fileType);
	std::string &getEditorBaseDir();
	std::string getResourcePath(const std::string &path);
};

#endif