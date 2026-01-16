#include "collisionsContainer.hpp"
#include <raylib.h>
#include <vector>

CollisionsContainer::CollisionsContainer() = default;

void CollisionsContainer::addCollisionTile(int x, int y) {
  vec.push_back(Vector2{static_cast<float>(x), static_cast<float>(y)});
}

void CollisionsContainer::removeCollisionTile(int x, int y) {
  int idx = 0;
  for (Vector2 item : vec) {
    if (item.x == static_cast<float>(x) && item.y == static_cast<float>(y)) {
      vec.erase(vec.begin() + idx);
    }
    idx++;
  }
}

const std::vector<Vector2> &CollisionsContainer::getVector() const {
  return vec;
}