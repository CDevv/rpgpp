#ifndef _RPGPP_COLLISIONSCONTAINER_H
#define _RPGPP_COLLISIONSCONTAINER_H

#include "baseContainer.hpp"
#include <raylib.h>
#include <vector>

/** A container of collision tiles to be used by a Room */
class CollisionsContainer : public BaseContainer<bool> {
  private:
	/** A vector that contains the tile positions of the collision tiles */

  public:
	/** Empty constructor */
	CollisionsContainer();
};

#endif