#include "room.hpp"
#include "collisionsContainer.hpp"
#include "interactable.hpp"
#include "interactablesContainer.hpp"
#include "tilemap.hpp"
#include <memory>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;

Room::Room()
{
    this->worldTileSize = 48;
    this->interactables = std::make_unique<InteractablesContainer>();
    this->collisions = std::make_unique<CollisionsContainer>();
    this->tileMap = std::unique_ptr<TileMap>{};
    this->actors = std::unique_ptr<std::vector<Actor>>{};
    this->player = std::unique_ptr<Player>{};
}

Room::Room(std::string fileName)
{
    this->worldTileSize = 48;

    this->interactables = std::make_unique<InteractablesContainer>();
    this->collisions = std::make_unique<CollisionsContainer>();
    this->actors = std::make_unique<std::vector<Actor>>();

    this->tileMap = std::make_unique<TileMap>(fileName);
    std::unique_ptr<Actor> actor = std::make_unique<Actor>("actors/playerActor.ractor");
    actor->setTilePosition(Vector2 {1, 0}, tileMap->getTileSet()->getTileSize());
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(actor), *this);

    this->addPlayer(std::move(player));

    char* jsonString = LoadFileText(fileName.c_str());
    json roomJson = json::parse(jsonString);

    std::vector<std::vector<int>> interactablesVec = roomJson.at("interactables");
    for (auto v : interactablesVec) {
        int x = v[0];
        int y = v[1];
        InteractableType itype = static_cast<InteractableType>(v[2]);

        interactables->add(x, y, itype);
    }

    std::vector<std::vector<int>> collisionsVec = roomJson.at("collision");
    for (auto v : collisionsVec) {
        int x = v[0];
        int y = v[1];

        collisions->addCollisionTile(x, y);
    }

    UnloadFileText(jsonString);
}

Room::Room(std::unique_ptr<TileMap> tileMap)
: Room()
{
    this->tileMap = std::move(tileMap);
}

Room::Room(RoomBin bin)
: Room()
{
    this->worldTileSize = 48;

    this->interactables = std::make_unique<InteractablesContainer>();
    this->collisions = std::make_unique<CollisionsContainer>();
    this->actors = std::make_unique<std::vector<Actor>>();

    this->tileMap = std::make_unique<TileMap>(bin);

    std::unique_ptr<Actor> actor = std::make_unique<Actor>(Game::getBin().actors.at(0));
    actor->setTilePosition(Vector2 {1, 0}, tileMap->getTileSet()->getTileSize());
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(actor), *this);
    
    this->addPlayer(std::move(player));

    for (auto intBin : bin.interactables) {
        InteractableType itype = static_cast<InteractableType>(intBin.type);
        interactables->add(intBin.x, intBin.y, itype);
    }

    interactables->add(1, 2, INT_TWO);
    interactables->get(1, 2)->setProp("text", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec placerat vel nulla eget ullamcorper.");

    for (auto collisionBin : bin.collisions) {
        collisions->addCollisionTile(collisionBin.x, collisionBin.y);
    }
}

json Room::dumpJson()
{
    json roomJson = this->tileMap->dumpJson();
    
    //Vector for collisions
    auto collisionsVector = std::vector<std::vector<int>>();
    for (Vector2 collisionPos : collisions->getVector()) {
        std::vector<int> collision;
        collision.push_back(collisionPos.x);
        collision.push_back(collisionPos.y);

        collisionsVector.push_back(collision);
    }
    roomJson.push_back({"collision", collisionsVector});

    //Vector for interactables
    auto interactablesVector = std::vector<std::vector<int>>();
    for (auto&& interactable : interactables->getVector()) {
        std::vector<int> interactableVector;
        interactableVector.push_back(interactable->getWorldPos().x);
        interactableVector.push_back(interactable->getWorldPos().y);
        interactableVector.push_back(static_cast<int>(interactable->getType()));

        interactablesVector.push_back(interactableVector);
    }
    roomJson.push_back({"interactables", interactablesVector});

    return roomJson;
}

void Room::unload()
{
    tileMap->unload();

    for (auto&& actor : *actors) {
        actor.unload();
    }

    player->unload();
}

void Room::update()
{
    for (auto&& actor : *actors) {
        actor.update();
    }
    player->update();
}

void Room::draw()
{
    this->tileMap->draw();
    for (auto i : interactables->getVector()) {
        Rectangle rect = i->getRect();
        DrawRectangleRec(rect, Fade(YELLOW, 0.5f));
    }
    for (auto c : collisions->getVector()) {
        Rectangle rect = Rectangle {
            c.x * worldTileSize, c.y * worldTileSize,
            static_cast<float>(worldTileSize), static_cast<float>(worldTileSize)
        };
        DrawRectangleRec(rect, Fade(RED, 0.5f));
    }

    for (auto&& actor : *actors) {
        actor.draw();
    }
    player->draw();
}

void Room::addActor(Actor actor) {
    this->actors->push_back(std::move(actor));
}

void Room::addPlayer(std::unique_ptr<Player> player)
{
    this->player = std::move(player);
}

Player& Room::getPlayer()
{
    return *player;
}

TileMap *Room::getTileMap()
{
    return this->tileMap.get();
}

std::vector<Vector2> Room::getCollisionTiles()
{
    return this->collisions->getVector();
}

std::vector<Interactable*> Room::getInteractableTiles()
{
    return this->interactables->getVector();
}

CollisionsContainer& Room::getCollisions()
{
    return *this->collisions;
}

InteractablesContainer& Room::getInteractables()
{
    return *this->interactables;
}

