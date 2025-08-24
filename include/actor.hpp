#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include "gamedata.hpp"
#include <memory>
#include <vector>
#include <array>
#include "atlasTile.hpp"
#include "tileset.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum Direction {
    RPGPP_DOWN_IDLE = 0,
    RPGPP_DOWN = 1,
    RPGPP_UP_IDLE = 2,
    RPGPP_UP = 3,
    RPGPP_LEFT_IDLE = 4,
    RPGPP_LEFT = 5,
    RPGPP_RIGHT_IDLE = 6,
    RPGPP_RIGHT = 7
};

class Actor {
private:
    std::unique_ptr<TileSet> tileSet;
    std::string tileSetSource;
    AtlasTile tile;
    Vector2 position;
    int frameCounter;
    int frameSpeed;
    int currentFrame;
    Rectangle collisionRect;
    std::array<std::unique_ptr<std::vector<Vector2>>, 8> animations;
    Direction currentAnimation;
public:
    Actor(std::string fileName);
    Actor(std::unique_ptr<TileSet> tileSet, Vector2 atlasPos);
    Actor(ActorBin bin);
    json dumpJson();
    void unload();
    void update();
    void draw();
    Vector2 getPosition();
    void setPosition(Vector2 position);
    Rectangle getRect();
    TileSet& getTileSet();
    void setTileSet(std::string tileSetSource);
    void moveByVelocity(Vector2 velocity);
    Rectangle getCollisionRect(Vector2 velocity);
    void addAnimation(Direction id, Vector2 atlasPos);
    void addAnimationFrames(Direction id, std::vector<std::vector<int>> frames);
    void changeAnimation(Direction id);
    std::string getTileSetSource();
    std::array<std::vector<Vector2>, 8> getAnimationsRaw();
    Rectangle getCollisionRect();
    void setCollisionRect(Rectangle rect);
};

#endif
