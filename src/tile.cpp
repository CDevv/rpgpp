#include "tile.hpp"
#include "atlasTile.hpp"
#include <raylib.h>

Tile::Tile() {
    this->worldCoords = Vector2 { 0, 0 };
    this->placed = false;
}

void Tile::place(AtlasTile atlasTile, Vector2 worldCoords) {
    this->atlasTile = atlasTile;
    this->worldCoords = worldCoords;
    this->placed = true;
}

bool Tile::isPlaced() {
    return this->placed;
}

AtlasTile Tile::getAtlasTile() {
    return this->atlasTile;
}

Vector2 Tile::getWorldCoords() {
    return this->worldCoords;
}
