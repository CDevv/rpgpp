#ifndef _RPGPP_ERASETILEACTION_H
#define _RPGPP_ERASETILEACTION_H

#include "mapAction.hpp"

class EraseTileAction : public MapAction {
  public:
	EraseTileAction(MapActionData data);
	void execute() override;
	void undo() override;
};

#endif
