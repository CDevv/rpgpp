#include "collisionsContainer.hpp"
#include <raylib.h>
#include <vector>

CollisionsContainer::CollisionsContainer() {}

void CollisionsContainer::addCollisionTile(int x, int y)
{
    vec.push_back(Vector2 {static_cast<float>(x), static_cast<float>(y)});
}

void CollisionsContainer::removeCollisionTile(int x, int y)
{
    for (std::vector<Vector2>::iterator it = vec.begin(); it != vec.end();) {
        if (it->x == x && it->y == y) {
            vec.erase(it);
        } else {
            ++it;
        }
    }
}

const std::vector<Vector2>& CollisionsContainer::getVector()
{
    return vec;
}