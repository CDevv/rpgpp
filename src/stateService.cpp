#include "stateService.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"
#include "actor.hpp"
#include "player.hpp"

StateService::StateService()
{
    Room *room = new Room("resources/map.json");

    this->room = room;
}

Room *StateService::getRoom()
{
    return room;
}

void StateService::unload()
{
    room->unload();
    delete room;
}

