#ifndef _RPGPP_WORLDSERVICE_H
#define _RPGPP_WORLDSERVICE_H

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
    Player& getPlayer();
    void update();
    void draw();
    void unload();
};

#endif
