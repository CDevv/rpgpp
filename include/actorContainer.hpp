#ifndef _RPGPP_ACTORCONTAINER_H
#define _RPGPP_ACTORCONTAINER_H

#include "actor.hpp"
#include "baseContainer.hpp"
class ActorContainer : public BaseContainer<Actor> {
  public:
	/** Construct the actor container. */
	ActorContainer() = default;
};

#endif // !_RPGPP_ACTORCONTAINER_H
