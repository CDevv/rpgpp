#ifndef _RPGPP_PROP_H
#define _RPGPP_PROP_H

#include <string>
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Prop {
private:
    Vector2 worldPos;
    Rectangle atlasRect;
    Texture2D texture;
    Rectangle collisionRect;
    std::string imagePath;
public:
    Prop();
    Prop(std::string filePath);
    Prop(Rectangle atlasRect, Vector2 worldPos);
    json dumpJson();
    void setTexture(Texture2D texture);
    Texture2D getTexture();
    void setTextureFromPath(std::string imagePath);
    const char* getImagePath();
    void setCollisionRect(Rectangle collisionRect);
    Vector2 getWorldPos();
    void setAtlasRect(Rectangle atlasRect);
    Rectangle getAtlasRect();
    Rectangle getWorldCollisionRect();
    Vector2 getCollisionCenter();
    void draw();
};

#endif
