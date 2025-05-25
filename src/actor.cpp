#include "actor.hpp"
#include <raylib.h>

Actor::Actor(TileSet *tileSet, Vector2 atlasPos) {
    this->tileSet = tileSet;
    this->tile = tileSet->getTile(atlasPos);

    this->frameCounter = 0;
    this->frameSpeed = 2;
    this->currentFrame = 0;
    this->frames.push_back((Vector2){ 0, 0 });
    this->frames.push_back((Vector2){ 1, 0 });
}

void Actor::unload() {
    tileSet->unload();
}

void Actor::update() {
    frameCounter++;

    if (frameCounter >= (60/frameSpeed)) {
        frameCounter = 0;
        currentFrame++;

        if (currentFrame >= 2) currentFrame = 0;

        float atlasTileSize = (float)tileSet->getTileSize();
        Vector2 atlasPos = frames.at(currentFrame);
        atlasPos = (Vector2){
            atlasPos.x * atlasTileSize,
            atlasPos.y * atlasTileSize
        };

        this->tile = tileSet->getTile(atlasPos);
    }
}

void Actor::draw() {
    //defaults..
    const Vector2 origin = (Vector2){ 0.0f, 0.0f };
    const float rotation = 0.0f;
    const float atlasTileSize = (float)tileSet->getTileSize();
    const float worldTileSize = 48.0f;

    //texture
    Texture texture = this->tileSet->getTexture();

    //actual coordinates
    Vector2 atlasCoords = this->tile.getAtlasCoords();

    //build rects
    Rectangle atlasRect = (Rectangle){
        atlasCoords.x, atlasCoords.y,
        atlasTileSize, atlasTileSize
    };
    Rectangle worldRect = (Rectangle){
        position.x, position.y,
        worldTileSize, worldTileSize
    };

    //draw it
    DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);
}
