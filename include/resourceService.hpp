#ifndef _RPGPP_EDITOR_RESOURCESERVICE_H
#define _RPGPP_EDITOR_RESOURCESERVICE_H

#include <string>
#include <map>
#include <raylib.h>

class ResourceService {
private:
    std::map<std::string, Texture2D> textures;
public:
    ResourceService();
    ~ResourceService();
    void addTextureFromFile(std::string filePath);
    void addTexture(std::string id, Texture2D texture);
    Texture2D getTexture(std::string id);
    void unload();
};

#endif
