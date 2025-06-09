#ifndef _RPGPP_WORLDSERVICE_H
#define _RPGPP_WORLDSERVICE_H

#include "room.hpp"
#include "player.hpp"

class WorldService {
private:
    Room *room;
public:
    WorldService();
    Room *getRoom();
    Player *getPlayer();
    void update();
    void draw();
    void unload();
};

#endif
