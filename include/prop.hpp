#ifndef _RPGPP_PROP_H
#define _RPGPP_PROP_H

#include "gamedata.hpp"
#include <string>
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Prop {
private:
    std::string sourcePath;
    Vector2 worldPos;
    Vector2 tilePos;
    Rectangle atlasRect;
    Texture2D texture;
    Rectangle collisionRect;
    std::string imagePath;
public:
    Prop();
    Prop(std::string filePath);
    Prop(Rectangle atlasRect, Vector2 worldPos);
    Prop(PropBin bin);
    json dumpJson();
    std::string getSourcePath();
    void setTexture(Texture2D texture);
    Texture2D getTexture();
    void setTextureFromPath(std::string imagePath);
    const char* getImagePath();
    void setCollisionRect(Rectangle collisionRect);
    void setWorldTilePos(Vector2 worldPos, int tileSize);
    void setWorldPos(Vector2 worldPos);
    Vector2 getWorldPos();
    Vector2 getWorldTilePos();
    void setAtlasRect(Rectangle atlasRect);
    Rectangle getAtlasRect();
    Rectangle getWorldCollisionRect();
    Vector2 getCollisionCenter();
    void draw();
};

#endif
