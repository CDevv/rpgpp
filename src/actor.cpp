#include "actor.hpp"
#include "game.hpp"
#include <raylib.h>
#include <raymath.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Actor::Actor(std::string fileName)
{
    this->position = Vector2 { 0, 0 };
    this->frameCounter = 0;
    this->frameSpeed = 2;
    this->currentFrame = 0;
    this->currentAnimation = RPGPP_DOWN_IDLE;

    char *jsonContent = LoadFileText(fileName.c_str());
    json j = json::parse(jsonContent);

    std::vector<int> collisionInfo = j.at("collision");
    int x = collisionInfo.at(0);
    int y = collisionInfo.at(1);
    int width = collisionInfo.at(2);
    int height = collisionInfo.at(3);

    Rectangle collisionRect = Rectangle {
        static_cast<float>(x), static_cast<float>(y),
        static_cast<float>(width), static_cast<float>(height)
    };
    this->collisionRect = collisionRect;

    std::string tileSetSource = j.at("tileset");
    this->tileSet = std::make_unique<TileSet>(tileSetSource);
    this->tileSetSource = tileSetSource;

    for (int i = 0; i < 8; i++) {
        //std::vector<Vector2> *frames = new std::vector<Vector2>;
        animations[i] = std::make_unique<std::vector<Vector2>>();
    }

    std::vector<std::vector<int>> down = j.at("animations").at("down");
    addAnimationFrames(RPGPP_DOWN, down);

    std::vector<std::vector<int>> downIdle = j.at("animations").at("down-idle");
    addAnimationFrames(RPGPP_DOWN_IDLE, downIdle);

    std::vector<std::vector<int>> up = j.at("animations").at("up");
    addAnimationFrames(RPGPP_UP, up);

    std::vector<std::vector<int>> upIdle = j.at("animations").at("up-idle");
    addAnimationFrames(RPGPP_UP_IDLE, upIdle);

    std::vector<std::vector<int>> left = j.at("animations").at("left");
    addAnimationFrames(RPGPP_LEFT, left);

    std::vector<std::vector<int>> leftIdle = j.at("animations").at("left-idle");
    addAnimationFrames(RPGPP_LEFT_IDLE, leftIdle);

    std::vector<std::vector<int>> right = j.at("animations").at("right");
    addAnimationFrames(RPGPP_RIGHT, right);

    std::vector<std::vector<int>> rightIdle = j.at("animations").at("right-idle");
    addAnimationFrames(RPGPP_RIGHT_IDLE, rightIdle);

    Vector2 defaultTileAtlasPos = animations[(int)currentAnimation]->at(0);
    this->tile = tileSet->getTile(defaultTileAtlasPos);

    UnloadFileText(jsonContent);
}

Actor::Actor(std::unique_ptr<TileSet> tileSet, Vector2 atlasPos)
{
    this->position = Vector2 { 0, 0 };

    this->tileSet = std::move(tileSet);
    this->tile = tileSet->getTile(atlasPos);

    this->frameCounter = 0;
    this->frameSpeed = 2;
    this->currentFrame = 0;
    this->currentAnimation = RPGPP_DOWN_IDLE;

    for (int i = 0; i < 8; i++) {
        animations[i] = std::make_unique<std::vector<Vector2>>();
    }
}

Actor::Actor(ActorBin bin)
{
    this->position = Vector2 { 0, 0 };

    this->frameCounter = 0;
    this->frameSpeed = 2;
    this->currentFrame = 0;
    this->currentAnimation = RPGPP_DOWN_IDLE;

    //tileset
    this->tileSet = std::make_unique<TileSet>(Game::getBin().tilesets.at(bin.tileSetName));

    //collision
    this->collisionRect = Rectangle
    {
        static_cast<float>(bin.collision.x), static_cast<float>(bin.collision.y),
        static_cast<float>(bin.collision.width), static_cast<float>(bin.collision.height)
    };

    //animations
    for (int i = 0; i < 8; i++) {
        animations[i] = std::make_unique<std::vector<Vector2>>();
    }
    for (int i = 0; i < 8; i++) {
        std::vector<IVector> binFrames = bin.animations[i];
        std::vector<std::vector<int>> frames;
        for (int j = 0; j < bin.animations[i].size(); j++) {
            Vector2 frame = Vector2
            {
                static_cast<float>(binFrames[j].x), static_cast<float>(binFrames[j].y)
            };
            std::vector<int> frameVec;
            frameVec.push_back(binFrames[j].x);
            frameVec.push_back(binFrames[j].y);
            frames.push_back(frameVec);
        }

        addAnimationFrames(static_cast<Direction>(i), frames);
    }
}

void Actor::unload()
{

}

void Actor::update()
{
    frameCounter++;

    if (frameCounter >= (60/frameSpeed)) {
        frameCounter = 0;
        currentFrame++;

        if (currentFrame >= 2) currentFrame = 0;

        Vector2 atlasTileSize = tileSet->getTileSize();
        int animId = static_cast<int>(currentAnimation);
        Vector2 atlasPos = animations[animId]->at(currentFrame);
        atlasPos = Vector2 {
            atlasPos.x * atlasTileSize.x,
            atlasPos.y * atlasTileSize.y
        };

        this->tile = tileSet->getTile(atlasPos);
    }
}

void Actor::draw()
{
    //defaults..
    const Vector2 origin = Vector2 { 0.0f, 0.0f };
    const float rotation = 0.0f;
    const Vector2 atlasTileSize = tileSet->getTileSize();
    const float worldTileSize = 48.0f;

    //texture
    Texture texture = this->tileSet->getTexture();

    //actual coordinates
    Vector2 atlasCoords = this->tile.getAtlasCoords();

    //build rects
    Rectangle atlasRect = Rectangle {
        atlasCoords.x, atlasCoords.y,
        atlasTileSize.x, atlasTileSize.y
    };
    Rectangle worldRect = Rectangle {
        position.x, position.y,
        worldTileSize, worldTileSize
    };

    //draw it
    DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);

    //draw collision rect..
    Color collisionDebugColor = GRAY;
    collisionDebugColor.a = (255 / 2);

    Rectangle collisionRect = getCollisionRect(Vector2 { 0, 0 });
    DrawRectanglePro(collisionRect, origin, rotation, collisionDebugColor);
}

Vector2 Actor::getPosition()
{
    return position;
}

Rectangle Actor::getRect()
{
    Vector2 atlasTileSize = tileSet->getTileSize();
    Rectangle result = Rectangle {
        position.x, position.y, 48, 48
    };
    return result;
}

TileSet& Actor::getTileSet()
{
    return *tileSet;
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

Rectangle Actor::getCollisionRect(Vector2 velocity)
{
    Vector2 newPos = Vector2Add(position, velocity);
    Rectangle result = Rectangle {
        newPos.x + collisionRect.x, newPos.y + collisionRect.y,
        collisionRect.width, collisionRect.height
    };

    return result;
}

void Actor::addAnimation(Direction id, Vector2 atlasPos)
{
    animations[static_cast<int>(id)]->push_back(atlasPos);
}

void Actor::addAnimationFrames(Direction id, std::vector<std::vector<int>> frames)
{
    int idNum = static_cast<int>(id);

    for (int i = 0; i < frames.size(); i++) {
        int x = frames.at(i).at(0);
        int y = frames.at(i).at(1);
        Vector2 pos = Vector2 { static_cast<float>(x), static_cast<float>(y) };

        animations[idNum]->push_back(pos);
    }
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

std::string Actor::getTileSetSource()
{
    return tileSetSource;
}

std::array<std::vector<Vector2>, 8> Actor::getAnimationsRaw()
{
    std::array<std::vector<Vector2>, 8> result;
    for (int i = 0; i < 8; i++) {
        std::vector<Vector2> animFrames = *animations[i];
        result[i] = animFrames;
    }
    return result;
}

Rectangle Actor::getCollisionRect()
{
    return collisionRect;
}
