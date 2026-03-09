#ifndef _RPGPP_COLLISIONSCONTAINER_H
#define _RPGPP_COLLISIONSCONTAINER_H

#include <raylib.h>
#include <vector>

/** A container of collision tiles to be used by a Room */
class CollisionsContainer {
  private:
	/** A vector that contains the tile positions of the collision tiles */
	std::vector<Vector2> vec;

  public:
	/** Empty constructor */
	CollisionsContainer();
	/** Add a collision tile to this container */
	void addCollisionTile(int x, int y);
	/** Remove a collision tile by its tile position */
	void removeCollisionTile(int x, int y);
	/** Get a reference to this container's vector. Items are tile positions of
	 * the collision tiles. */
	const std::vector<Vector2> &getVector() const;
};

#endif