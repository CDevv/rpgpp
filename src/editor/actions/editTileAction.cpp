
#include "actions/editTileAction.hpp"
#include "actions/mapAction.hpp"
#include "tilemap.hpp"

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
