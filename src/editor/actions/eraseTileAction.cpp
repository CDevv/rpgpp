
#include "actions/eraseTileAction.hpp"
#include "widgets/roomToolbox.hpp"
EraseTileAction::EraseTileAction(Room *room, RoomLayer layer, IVector worldPos,
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

void EraseTileAction::execute() {
	switch (layer) {
	case RoomLayer::LAYER_TILES: {
		room->getTileMap()->setEmptyTile(worldPos);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		room->getCollisions().removeCollisionTile(static_cast<int>(worldPos.x),
												  static_cast<int>(worldPos.y));
	} break;
	default:
		break;
	}
}

void EraseTileAction::undo() {
	switch (layer) {
	case RoomLayer::LAYER_TILES: {
		room->getTileMap()->setTile(worldPos, prevTile);
	} break;
	case RoomLayer::LAYER_COLLISION: {
		room->getCollisions().addCollisionTile(static_cast<int>(worldPos.x),
											   static_cast<int>(worldPos.y));
	} break;
	default:
		break;
	}
}
