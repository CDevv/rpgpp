#include "worldService.hpp"

WorldService::WorldService()
{
    this->lock = false;
    this->room = std::unique_ptr<Room>{};
    this->deferRoomChange = false;
    this->deferredRoomId = "";
}

Room& WorldService::getRoom()
{
    return *this->room;
}

void WorldService::setRoom(std::string filePath)
{
    for (RoomBin bin : Game::getBin().rooms) {
        if (bin.name == filePath) {
            deferRoomChange = true;
            deferredRoomId = bin.name;
            break;
        }
    }
}

void WorldService::setRoomBin(RoomBin bin)
{
    this->room = std::make_unique<Room>(bin);
}

Player& WorldService::getPlayer()
{
    return this->room->getPlayer();
}

void WorldService::update()
{
    if (deferRoomChange) {
        for (RoomBin bin : Game::getBin().rooms) {
            if (bin.name == deferredRoomId) {
                setRoomBin(bin);
                break;
            }
        }
        deferRoomChange = false;
    } else {
        room->update();
    }
}

void WorldService::draw()
{
    room->draw();
}

void WorldService::unload()
{
    //room->unload();
}
