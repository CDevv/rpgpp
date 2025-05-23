#ifndef _RPGPP_ATLASTILE_H
#define _RPGPP_ATLASTILE_H

#include <raylib.h>

class AtlasTile {
private:
    Texture *texture;
    Vector2 atlasCoords;
public:
    AtlasTile();
    AtlasTile(Texture *texture, Vector2 atlasCoords);
    Vector2 getAtlasCoords();
};

#endif
