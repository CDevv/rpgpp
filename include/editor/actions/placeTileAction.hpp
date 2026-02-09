#ifndef _RPGPP_PLACETILEACTION_H
#define _RPGPP_PLACETILEACTION_H

#include "action.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tilemap.hpp"
#include "widgets/roomToolbox.hpp"

class PlaceTileAction : public Action {
  public:
	Room *room;
	TileMap *tileMap;
	RoomLayer layer;
	InteractableType interactable;
	Vector2 worldTile;
	Vector2 tile;
	Vector2 prevTile;
	PlaceTileAction(Room *room, TileMap *tileMap, RoomLayer layer,
					IVector worldTile, IVector tile);
	void execute() override;
	void undo() override;
};

#endif