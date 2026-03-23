#ifndef _RPGPP_COLLISIONSCONTAINER_H
#define _RPGPP_COLLISIONSCONTAINER_H

#include "baseContainer.hpp"
#include "gamedata.hpp"
#include <raylib.h>

/** A container of collision tiles to be used by a Room */
class CollisionsContainer : public BaseContainer<bool> {
  public:
	/** Empty constructor */
	CollisionsContainer();
	/** Add a collision */
	void pushCollision(IVector pos);
};

#endif