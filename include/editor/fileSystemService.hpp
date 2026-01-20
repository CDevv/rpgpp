#ifndef _RPGPP_FILESYSTEMSERVICE_H
#define _RPGPP_FILESYSTEMSERVICE_H

#include <array>
#include <string>
enum class EngineFileType { FILE_TILESET, FILE_MAP };

class FileSystemService {
  private:
    std::array<std::string, 2> typeNames;
  public:
	FileSystemService();
	void unload();
	void promptOpenProject();
    std::string &getTypeName(EngineFileType fileType);
};

#endif