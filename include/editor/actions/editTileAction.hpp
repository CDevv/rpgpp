#ifndef _RPGPP_EDITTILEACTION_H
#define _RPGPP_EDITTILEACTION_H

#include "actions/action.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tileSetView.hpp"
#include "widgets/roomToolbox.hpp"

class EditTileAction : public Action {
  public:
	Room *room;
	RoomLayer layer;
	Vector2 worldPos;
	Vector2 tilePos;
	Vector2 prevTile;
	EditTileAction(Room *room, RoomLayer layer, IVector worldPos,
				   IVector tilePos);
	void execute() override;
	void undo() override;

	TileSetView *tileSetView;
	void init();
};

#endif