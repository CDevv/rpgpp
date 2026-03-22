
#include "actions/eraseTileAction.hpp"
#include "actions/mapAction.hpp"
#include "conversion.hpp"
#include "views/worldView.hpp"

EraseTileAction::EraseTileAction(MapActionData a) : MapAction(a) {}

void EraseTileAction::execute() {
	switch (data.layer) {
	case RoomLayer::LAYER_TILES: {
		data.room->getTileMap()->setEmptyTile(data.worldTile);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		auto conv = fromVector2(data.worldTile);
		data.room->getCollisions().removeObject(fromVector2(data.worldTile));
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		data.room->getInteractables().removeObject(fromVector2(data.worldTile));
	} break;
	case RoomLayer::LAYER_PROPS: {
		data.room->getProps().removeObject(fromVector2(data.worldTile));
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
		data.room->getCollisions().pushObject(fromVector2(data.worldTile),
											  false);
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {

	} break;
	default:
		break;
	}
}
