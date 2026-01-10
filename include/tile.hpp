#ifndef _RPGPP_TILE_H
#define _RPGPP_TILE_H

#include "atlasTile.hpp"
#include <raylib.h>

/**
 * A tile that may be placed in the world.
 */
class Tile {
private:
    /** The Tile's source AtlasTile. */
    AtlasTile atlasTile;
    /** The Tile's World coordinates. */
    Vector2 worldCoords{};
    /** Whether this Tile is placed and will be drawn or not. */
    bool placed;
public:
    Tile();

    /**
     * Check if it was placed or not.
     */
    bool isPlaced() const;

    /**
     * Place the tile using a source AtlasTile and world coordinates.
     * @param atlasTile The source atlas tile from a TileSet.
     * @param worldCoords A Vector2, representing the coordinates on the world where it will be placed.
     */
    void place(AtlasTile atlasTile, Vector2 worldCoords);

    /**
     * Erase the tile, no longer to be shown in-game.
     */
    void erase();

    /**
     * Get the source AtlasTile that originates from the used TileSet.
     * @returns The source AtlasTile.
     */
    AtlasTile getAtlasTile() const;

    /**
     * Get the world coordinates, at which the Tile is placed.
     * @returns The world coordinates.
     */
    Vector2 getWorldCoords() const;
};

#endif
