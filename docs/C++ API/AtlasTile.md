---
generator: doxide
---


# AtlasTile

**class AtlasTile**

Represents a source atlas tile from a TileSet.


## Variables

| Name | Description |
| ---- | ----------- |
| [texture](#texture) | Pointer to the used Texture  |
| [atlasCoords](#atlasCoords) | The atlas (source) coordinates from the TileSet. |

## Functions

| Name | Description |
| ---- | ----------- |
| [AtlasTile](#AtlasTile) | Empty constructor  |
| [AtlasTile](#AtlasTile) | Constructor that takes in a texture and coordinates on the atlas (the tileset)  |
| [getAtlasCoords](#getAtlasCoords) | Get the atlas coordinates :material-location-exit: **Return** :    A Vector2 which represents the atlas coordinates  |

## Variable Details

### atlasCoords<a name="atlasCoords"></a>

!!! variable "Vector2 atlasCoords"

    The atlas (source) coordinates from the TileSet.
    

### texture<a name="texture"></a>

!!! variable "Texture &#42;texture"

    Pointer to the used Texture
    

## Function Details

### AtlasTile<a name="AtlasTile"></a>
!!! function "AtlasTile()"

    Empty constructor
    

!!! function "AtlasTile(Texture &#42;texture, Vector2 atlasCoords)"

    Constructor that takes in a texture and coordinates on the atlas (the tileset)
    

### getAtlasCoords<a name="getAtlasCoords"></a>
!!! function "Vector2 getAtlasCoords()"

    Get the atlas coordinates
    
    :material-location-exit: **Return**
    :    A Vector2 which represents the atlas coordinates
    

