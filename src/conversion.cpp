#include "conversion.hpp"

#include "gamedata.hpp"
#include "raylib.h"

Vector2 fromIVector(const IVector &other) { return {static_cast<float>(other.x), static_cast<float>(other.y)}; }

IVector fromVector2(const Vector2 &other) { return {static_cast<int>(other.x), static_cast<int>(other.y)}; }