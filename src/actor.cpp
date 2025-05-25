#include "actor.hpp"
#include <raylib.h>
#include <raymath.h>

Actor::Actor(TileSet *tileSet, Vector2 atlasPos) {
    this->tileSet = tileSet;
    this->tile = tileSet->getTile(atlasPos);

    this->frameCounter = 0;
    this->frameSpeed = 2;
    this->currentFrame = 0;
    this->currentAnimation = RPGPP_DOWN_IDLE;

    for (int i =0; i < 8; i++) {
        std::vector<Vector2> *frames = new std::vector<Vector2>;
        animations[i] = frames;
    }
}

void Actor::unload() {
    tileSet->unload();

    for (int i = 0; i < 8; i++) {
        delete animations[i];
    }
}

void Actor::update() {
    frameCounter++;

    if (frameCounter >= (60/frameSpeed)) {
        frameCounter = 0;
        currentFrame++;

        if (currentFrame >= 2) currentFrame = 0;

        float atlasTileSize = (float)tileSet->getTileSize();
        Vector2 atlasPos = animations[(int)currentAnimation]->at(currentFrame);
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

Vector2 Actor::getPosition()
{
    return position;
}

void Actor::setPosition(Vector2 position)
{
    this->position = position;
}

void Actor::moveByVelocity(Vector2 velocity)
{
    Vector2 resultVector = Vector2Add(position, velocity);

    this->position = resultVector;
}

void Actor::addAnimation(Direction id, Vector2 atlasPos)
{
    animations[(int)id]->push_back(atlasPos);
}

void Actor::changeAnimation(Direction id)
{
    if (this->currentAnimation != id) {
        this->currentFrame = 0;
        this->currentAnimation = id;

        //change to the first frame of the new animation immidiately
        frameCounter = (60/frameSpeed);
    }
}

