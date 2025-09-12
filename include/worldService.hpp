#ifndef _RPGPP_WORLDSERVICE_H
#define _RPGPP_WORLDSERVICE_H

#include "game.hpp"
#include "gamedata.hpp"
#include <memory>
#include "room.hpp"
#include "player.hpp"
#include <string>

/** The WorldService is responsible for containing and drawing the Room. */
class WorldService {
private:
    /** The current room in this World. */
    std::unique_ptr<Room> room;
public:
    /** Empty constructor. */
    WorldService();
    /** Get a reference to the current room. */
    Room& getRoom();
    /** Set the current room using path to a .rmap file. */
    void setRoom(std::string filePath);
    /** Set the current room using a RoomBin binary structure. */
    void setRoomBin(RoomBin bin);
    /** Get a reference to the Player object. */
    Player& getPlayer();
    /** Update routine. */
    void update();
    /** Draw routine. */
    void draw();
    /** Unload routine. */
    void unload();
};

#endif
