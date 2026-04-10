#include "collisionsContainer.hpp"

#include <raylib.h>

#include "gamedata.hpp"

CollisionsContainer::CollisionsContainer() = default;

void CollisionsContainer::pushCollision(IVector pos) { pushObject(pos, true); }
