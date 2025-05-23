#include "room.hpp"
#include "tilemap.hpp"

Room::Room(TileMap *tileMap) {
    this->tileMap = tileMap;
}

Room::~Room() {
    delete this->tileMap;
}

void Room::draw() {
    this->tileMap->draw();
}
