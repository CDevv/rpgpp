#include "room.hpp"
#include "tilemap.hpp"

Room::Room(TileMap *tileMap) {
    this->tileMap = tileMap;
}

Room::~Room() {
    delete this->tileMap;
}

void Room::addActor(Actor actor) {
    this->actors.push_back(actor);
}

void Room::draw() {
    this->tileMap->draw();

    for (Actor actor : actors) {
        actor.draw();
    }
}
