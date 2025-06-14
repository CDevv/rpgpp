#include "worldService.hpp"

WorldService::WorldService()
{
    this->room = std::unique_ptr<Room>{};
}

Room& WorldService::getRoom()
{
    return *this->room;
}

void WorldService::setRoom(std::string filePath)
{
    this->room = std::make_unique<Room>(filePath);
}

Player& WorldService::getPlayer()
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
}

