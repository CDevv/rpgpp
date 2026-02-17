#ifndef _RPGPP_EDITTILEACTION_H
#define _RPGPP_EDITTILEACTION_H

#include "mapAction.hpp"
#include "views/tileSetView.hpp"

class EditTileAction : public MapAction {
  public:
	EditTileAction(MapActionData a);
	void execute() override;
	void undo() override;

	TileSetView *tileSetView;
	void init();
};

#endif
