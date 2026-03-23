#include "collisionsContainer.hpp"
#include "gamedata.hpp"
#include <raylib.h>

CollisionsContainer::CollisionsContainer() = default;

void CollisionsContainer::pushCollision(IVector pos) { pushObject(pos, true); }
