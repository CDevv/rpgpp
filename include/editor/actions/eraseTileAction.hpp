#ifndef _RPGPP_ERASETILEACTION_H
#define _RPGPP_ERASETILEACTION_H

#include "actions/action.hpp"
#include "gamedata.hpp"
#include "mapAction.hpp"
#include "raylib.h"
#include "room.hpp"
#include "widgets/roomToolbox.hpp"

class EraseTileAction : public MapAction {
  public:
	EraseTileAction(MapActionData data);
	void execute() override;
	void undo() override;
};

#endif