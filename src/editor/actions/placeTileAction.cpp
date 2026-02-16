#include "actions/placeTileAction.hpp"
#include "actions/mapAction.hpp"
#include "raylib.h"
#include "room.hpp"

PlaceTileAction::PlaceTileAction(MapActionData a) : MapAction(a) {}

void PlaceTileAction::execute() {
	switch (data.layer) {
	case RoomLayer::LAYER_TILES: {
		if (data.room->getTileMap()->getTileSet()->areAtlasCoordsValid(
				{static_cast<float>(data.tile.x),
				 static_cast<float>(data.tile.y)})) {
			data.room->getTileMap()->setTile(data.worldTile, data.tile);
		}
	} break;
	case RoomLayer::LAYER_COLLISION: {
		data.room->getCollisions().addCollisionTile(data.worldTile.x,
													data.worldTile.y);
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		data.room->getInteractables().add(data.worldTile.x, data.worldTile.y,
										  data.interactable);
	} break;
	default:
		break;
	}
}

void PlaceTileAction::undo() {
	switch (data.layer) {
	case RoomLayer::LAYER_TILES: {
		data.room->getTileMap()->setTile(data.worldTile, data.prevTile);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		data.room->getCollisions().removeCollisionTile(data.worldTile.x,
													   data.worldTile.y);
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		data.room->getInteractables().removeInteractable(data.worldTile.x,
														 data.worldTile.y);
	} break;
	default:
		break;
	}
}
