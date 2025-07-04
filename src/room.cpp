#include "room.hpp"
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
    this->interactables = std::unique_ptr<InteractablesContainer>{};
    this->tileMap = std::unique_ptr<TileMap>{};
    this->actors = std::unique_ptr<std::vector<Actor>>{};
    this->player = std::unique_ptr<Player>{};
}

Room::Room(std::string fileName)
{
    this->interactables = std::make_unique<InteractablesContainer>();
    this->actors = std::make_unique<std::vector<Actor>>();

    std::unique_ptr<Actor> actor = std::make_unique<Actor>("resources/playerActor.json");
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(actor), *this);

    this->tileMap = std::make_unique<TileMap>(fileName);
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

    UnloadFileText(jsonString);
}

Room::Room(std::unique_ptr<TileMap> tileMap)
{
    this->tileMap = std::move(tileMap);
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
        Rectangle rect = i.getRect();
        DrawRectangleRec(rect, Fade(YELLOW, 0.5f));
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

TileMap& Room::getTileMap()
{
    return *this->tileMap;
}

std::vector<Vector2> Room::getCollisionTiles()
{
    return this->tileMap->getCollisionTiles();
}

std::vector<Interactable> Room::getInteractableTiles()
{
    return this->interactables->getVector();
}



