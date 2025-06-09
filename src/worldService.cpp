#include "worldService.hpp"

WorldService::WorldService()
{
    this->room = nullptr;
}

Room *WorldService::getRoom()
{
    return this->room;
}

void WorldService::setRoom(std::string filePath)
{
    Room *room = new Room(filePath);

    this->room = room;
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

