#ifndef _RPGPP_FILESYSTEMSERVICE_H
#define _RPGPP_FILESYSTEMSERVICE_H

#include "variant.hpp"
#include <array>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#define FILETYPE_MAX 2

enum class EngineFileType { FILE_TILESET, FILE_MAP };

class FileSystemService {
  private:
	std::array<std::string, FILETYPE_MAX> typeNames;

  public:
	FileSystemService();
	void unload();
	void promptOpenProject();
	std::string &getTypeName(EngineFileType fileType);
};

#endif