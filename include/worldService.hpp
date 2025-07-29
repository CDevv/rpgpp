#ifndef _RPGPP_WORLDSERVICE_H
#define _RPGPP_WORLDSERVICE_H

#include "game.hpp"
#include "gamedata.hpp"
#include <memory>
#include "room.hpp"
#include "player.hpp"
#include <string>

class WorldService {
private:
    std::unique_ptr<Room> room;
public:
    WorldService();
    Room& getRoom();
    void setRoom(std::string filePath);
    void setRoomBin(RoomBin bin);
    Player& getPlayer();
    void update();
    void draw();
    void unload();
};

#endif
