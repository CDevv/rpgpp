
#include "actions/eraseTileAction.hpp"
#include "actions/mapAction.hpp"

EraseTileAction::EraseTileAction(MapActionData a) : MapAction(a) {}

void EraseTileAction::execute() {
	switch (data.layer) {
	case RoomLayer::LAYER_TILES: {
		data.room->getTileMap()->setEmptyTile(data.worldTile);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		data.room->getCollisions().removeCollisionTile(
			static_cast<int>(data.worldTile.x),
			static_cast<int>(data.worldTile.y));
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		data.room->getInteractables().removeInteractable(
			static_cast<int>(data.worldTile.x),
			static_cast<int>(data.worldTile.y));
	} break;
	default:
		break;
	}
}

void EraseTileAction::undo() {
	switch (data.layer) {
	case RoomLayer::LAYER_TILES: {
		data.room->getTileMap()->setTile(data.worldTile, data.prevTile);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		data.room->getCollisions().addCollisionTile(
			static_cast<int>(data.worldTile.x),
			static_cast<int>(data.worldTile.y));
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {

	} break;
	default:
		break;
	}
}
