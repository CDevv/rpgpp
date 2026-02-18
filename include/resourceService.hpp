#ifndef _RPGPP_EDITOR_RESOURCESERVICE_H
#define _RPGPP_EDITOR_RESOURCESERVICE_H

#include <map>
#include <raylib.h>
#include <string>

class ResourceService {
  private:
	std::map<std::string, Texture2D> textures;

  public:
	ResourceService();
	~ResourceService();
	void addTextureFromFile(const std::string &filePath);
	void addTexture(const std::string &id, Texture2D texture);
	Texture2D getTexture(const std::string &id);
	void unload() const;
};

#endif
