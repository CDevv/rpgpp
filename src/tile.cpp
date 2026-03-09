#include "tile.hpp"
#include "atlasTile.hpp"
#include <raylib.h>

Tile::Tile() {
	this->worldCoords = Vector2{-1, -1};
	this->placed = false;
}

void Tile::place(AtlasTile atlas_to_set, Vector2 world_coords_to_set) {
	this->atlasTile = atlas_to_set;
	this->worldCoords = world_coords_to_set;
	this->placed = true;
}

void Tile::erase() { this->placed = false; }

bool Tile::isPlaced() const { return this->placed; }

AtlasTile Tile::getAtlasTile() const { return this->atlasTile; }

Vector2 Tile::getWorldCoords() const { return this->worldCoords; }
