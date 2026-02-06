#ifndef _RPGPP_ERASETILEACTION_H
#define _RPGPP_ERASETILEACTION_H

#include "actions/action.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "room.hpp"
#include "widgets/roomToolbox.hpp"

class EraseTileAction : public Action {
  public:
	Room *room;
	RoomLayer layer;
	Vector2 worldPos;
	Vector2 tilePos;
	Vector2 prevTile;
	EraseTileAction(Room *room, RoomLayer layer, IVector worldPos,
					IVector tilePos);
	void execute() override;
	void undo() override;
};

#endif