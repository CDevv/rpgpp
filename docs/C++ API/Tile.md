---
generator: doxide
---


# Tile

**class Tile**

A tile that may be placed in the world.


## Variables

| Name | Description |
| ---- | ----------- |
| [atlasTile](#atlasTile) | The Tile's source AtlasTile. |
| [worldCoords](#worldCoords) | The Tile's World coordinates. |
| [placed](#placed) | Whether this Tile is placed and will be drawn or not. |

## Functions

| Name | Description |
| ---- | ----------- |
| [isPlaced](#isPlaced) | Check if it was placed or not. |
| [place](#place) | Place the tile using a source AtlasTile and world coordinates. |
| [erase](#erase) | Erase the tile, no longer to be shown in-game. |
| [getAtlasTile](#getAtlasTile) | Get the source AtlasTile that originates from the used TileSet. |
| [getWorldCoords](#getWorldCoords) | Get the world coordinates, at which the Tile is placed. |

## Variable Details

### atlasTile<a name="atlasTile"></a>

!!! variable "AtlasTile atlasTile"

    The Tile's source AtlasTile.
    

### placed<a name="placed"></a>

!!! variable "bool placed"

    Whether this Tile is placed and will be drawn or not.
    

### worldCoords<a name="worldCoords"></a>

!!! variable "Vector2 worldCoords"

    The Tile's World coordinates.
    

## Function Details

### erase<a name="erase"></a>
!!! function "void erase()"

    Erase the tile, no longer to be shown in-game.
    

### getAtlasTile<a name="getAtlasTile"></a>
!!! function "AtlasTile getAtlasTile()"

    Get the source AtlasTile that originates from the used TileSet.
        
    :material-location-exit: **Return**
    :    The source AtlasTile.
    

### getWorldCoords<a name="getWorldCoords"></a>
!!! function "Vector2 getWorldCoords()"

    Get the world coordinates, at which the Tile is placed.
        
    :material-location-exit: **Return**
    :    The world coordinates.
    

### isPlaced<a name="isPlaced"></a>
!!! function "bool isPlaced()"

    Check if it was placed or not.
    

### place<a name="place"></a>
!!! function "void place(AtlasTile atlasTile, Vector2 worldCoords)"

    Place the tile using a source AtlasTile and world coordinates.
    
    :material-location-enter: `atlasTile`
    :    The source atlas tile from a TileSet.
        
    :material-location-enter: `worldCoords`
    :    A Vector2, representing the coordinates on the world where it will be placed.
    

