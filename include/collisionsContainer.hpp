#ifndef _RPGPP_COLLISIONSCONTAINER_H
#define _RPGPP_COLLISIONSCONTAINER_H

#include <vector>
#include <raylib.h>

class CollisionsContainer {
private:
    std::vector<Vector2> vec;
public:
    CollisionsContainer();
    void addCollisionTile(int x, int y);
    void removeCollisionTile(int x, int y);
    const std::vector<Vector2>& getVector();
};

#endif