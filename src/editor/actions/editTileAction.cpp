
#include "actions/editTileAction.hpp"
#include "gamedata.hpp"
#include "tilemap.hpp"
#include "widgets/roomToolbox.hpp"
EditTileAction::EditTileAction(Room *room, RoomLayer layer, IVector worldPos,
							   IVector tilePos) {
	this->room = room;
	this->layer = layer;
	this->worldPos = {static_cast<float>(worldPos.x),
					  static_cast<float>(worldPos.y)};
	this->tilePos = {static_cast<float>(tilePos.x),
					 static_cast<float>(tilePos.y)};

	this->prevTile = room->getTileMap()
						 ->getTile(worldPos.x, worldPos.y)
						 .getAtlasTile()
						 .getAtlasCoords();
}

void EditTileAction::init() {}

void EditTileAction::execute() {
	TileMap *tileMap = room->getTileMap();

	tileMap->setTile(worldPos, tilePos);
}

void EditTileAction::undo() {
	TileMap *tileMap = room->getTileMap();

	tileMap->setTile(worldPos, prevTile);
}