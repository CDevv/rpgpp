#ifndef _RPGPP_PLACETILEACTION_H
#define _RPGPP_PLACETILEACTION_H

#include "action.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "mapAction.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tilemap.hpp"
#include "widgets/roomToolbox.hpp"

class PlaceTileAction : public MapAction {
  public:
	PlaceTileAction(MapActionData a);
	void execute() override;
	void undo() override;
};

#endif