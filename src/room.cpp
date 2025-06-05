#include "room.hpp"
#include "tilemap.hpp"

Room::Room()
{
}

Room::Room(std::string fileName)
{
    TileMap *tileMap = new TileMap(fileName);
    Actor *actor = new Actor("resources/playerActor.json");
    Player *player = new Player(actor);

    this->tileMap = tileMap;
    this->addPlayer(player);
}

Room::Room(TileMap *tileMap)
{
    this->tileMap = tileMap;
}

void Room::unload()
{
    tileMap->unload();
    for (Actor *actor : actors) {
        actor->unload();
    }
    player->unload();
}

void Room::update()
{
    for (Actor *actor : actors) {
        actor->update();
    }
    player->update();
}

void Room::draw()
{
    this->tileMap->draw();
    for (Actor *actor : actors) {
        actor->draw();
    }
    player->draw();
}

void Room::addActor(Actor *actor) {
    this->actors.push_back(actor);
}

void Room::addPlayer(Player* player)
{
    this->player = player;
    //this->player->setRoom(this);
}

TileMap * Room::getTileMap()
{
    return this->tileMap;
}

std::vector<Vector2> Room::getCollisionTiles()
{
    return this->tileMap->getCollisionTiles();
}

std::vector<Interactable*> Room::getInteractableTiles()
{
    return this->tileMap->getInteractables();
}



