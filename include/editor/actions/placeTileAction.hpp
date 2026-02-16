#ifndef _RPGPP_PLACETILEACTION_H
#define _RPGPP_PLACETILEACTION_H

#include "mapAction.hpp"

class PlaceTileAction : public MapAction {
  public:
	PlaceTileAction(MapActionData a);
	void execute() override;
	void undo() override;
};

#endif
