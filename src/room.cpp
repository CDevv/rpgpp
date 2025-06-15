#include "room.hpp"
#include "tilemap.hpp"

Room::Room()
{
    this->tileMap = std::unique_ptr<TileMap>{};
    this->actors = std::unique_ptr<std::vector<Actor>>{};
    this->player = std::unique_ptr<Player>{};
}

Room::Room(std::string fileName)
{
    this->actors = std::make_unique<std::vector<Actor>>();

    std::unique_ptr<Actor> actor = std::make_unique<Actor>("resources/playerActor.json");
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(actor), *this);

    this->tileMap = std::make_unique<TileMap>(fileName);
    this->addPlayer(std::move(player));
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
    return this->tileMap->getInteractables();
}



