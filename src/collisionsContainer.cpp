#include "collisionsContainer.hpp"
#include <raylib.h>
#include <vector>

CollisionsContainer::CollisionsContainer() {}

void CollisionsContainer::addCollisionTile(int x, int y)
{
    vec.push_back(Vector2 {static_cast<float>(x), static_cast<float>(y)});
}

const std::vector<Vector2>& CollisionsContainer::getVector()
{
    return vec;
}