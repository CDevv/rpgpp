#include "worldService.hpp"

WorldService::WorldService()
{
    Room *room = new Room("resources/map.json");

    this->room = room;
}

Room *WorldService::getRoom()
{
    return this->room;
}

Player *WorldService::getPlayer()
{
    return this->room->getPlayer();
}

void WorldService::update()
{
    room->update();
}

void WorldService::draw()
{
    room->draw();
}

void WorldService::unload()
{
    room->unload();
    delete room;
}

