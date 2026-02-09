
#include "actions/editTileAction.hpp"
#include "gamedata.hpp"
#include "mapAction.hpp"
#include "tilemap.hpp"
#include "widgets/roomToolbox.hpp"

EditTileAction::EditTileAction(MapActionData a) : MapAction(a) {}

void EditTileAction::init() {}

void EditTileAction::execute() {
	TileMap *tileMap = data.room->getTileMap();

	tileMap->setTile(data.worldTile, data.tile);
}

void EditTileAction::undo() {
	TileMap *tileMap = data.room->getTileMap();

	tileMap->setTile(data.worldTile, data.prevTile);
}