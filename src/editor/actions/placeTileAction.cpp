#include "actions/placeTileAction.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "room.hpp"
#include "widgets/roomToolbox.hpp"

PlaceTileAction::PlaceTileAction(Room *room, TileMap *tileMap, RoomLayer layer,
								 IVector worldTile, IVector tile) {
	this->room = room;
	this->tileMap = tileMap;
	this->layer = layer;
	this->worldTile = {static_cast<float>(worldTile.x),
					   static_cast<float>(worldTile.y)};
	this->tile = {static_cast<float>(tile.x), static_cast<float>(tile.y)};

	this->prevTile = tileMap->getTile(worldTile.x, worldTile.y)
						 .getAtlasTile()
						 .getAtlasCoords();
}

void PlaceTileAction::execute() {
	switch (layer) {
	case RoomLayer::LAYER_TILES: {
		if (tileMap->getTileSet()->areAtlasCoordsValid(
				{static_cast<float>(tile.x), static_cast<float>(tile.y)})) {
			tileMap->setTile(worldTile, tile);
		}
	} break;
	case RoomLayer::LAYER_COLLISION: {
		room->getCollisions().addCollisionTile(worldTile.x, worldTile.y);
	} break;
	default:
		break;
	}
}

void PlaceTileAction::undo() {
	switch (layer) {
	case RoomLayer::LAYER_TILES: {
		tileMap->setTile(worldTile, prevTile);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		room->getCollisions().removeCollisionTile(worldTile.x, worldTile.y);
	} break;
	default:
		break;
	}
}
