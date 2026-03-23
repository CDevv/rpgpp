#ifndef _RPGPP_ACTORCONTAINER_H
#define _RPGPP_ACTORCONTAINER_H

#include "actor.hpp"

class ActorContainer {
  private:
	std::map<std::string, std::unique_ptr<Actor>> actors;

  public:
	/** Construct the actor container. */
	ActorContainer();
	/** Get the map itself */
	std::map<std::string, std::unique_ptr<Actor>> &getActors();
	/** Get an Actor with the specified name */
	Actor &getActor(const std::string &name);
	/** Add a new Actor with a name from the GameBin and an in-room name*/
	void addActor(Vector2 pos, const std::string &typeName,
				  const std::string &roomName);
	/** Remove an Actor */
	void removeActor(const std::string &roomName);
	/** Check whether an Actor with such an in-room name exists. */
	bool actorExists(const std::string &roomName);
};

#endif // !_RPGPP_ACTORCONTAINER_H
