#ifndef _RPGPP_ATLASTILE_H
#define _RPGPP_ATLASTILE_H

#include <raylib.h>

/**
 * Represents a source atlas tile from a TileSet.
 */
class AtlasTile {
private:
    Texture *texture;
    Vector2 atlasCoords;
public:
    /**
     * Empty constructor
     */
    AtlasTile();

    /**
     * Constructor that takes in a texture and coordinates on the atlas (the tileset)
     */
    AtlasTile(Texture *texture, Vector2 atlasCoords);

    /**
     * Get the atlas coordinates
     * @returns A Vector2 which represents the atlas coordinates
     */
    Vector2 getAtlasCoords();
};

#endif
