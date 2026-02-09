#ifndef _RPGPP_EDITTILEACTION_H
#define _RPGPP_EDITTILEACTION_H

#include "actions/action.hpp"
#include "mapAction.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tileSetView.hpp"
#include "widgets/roomToolbox.hpp"

class EditTileAction : public MapAction {
  public:
	EditTileAction(MapActionData a);
	void execute() override;
	void undo() override;

	TileSetView *tileSetView;
	void init();
};

#endif