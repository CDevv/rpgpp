---
generator: doxide
---


# TileSet

**class TileSet**

The TileSet class represents a collection of tiles that can be used for a TileMap or another object

:material-eye-outline: **See**
:    [TileMap](TileMap.md)


## Variables

| Name | Description |
| ---- | ----------- |
| [texture](#texture) | The used Texture for this TileSet. |
| [tileSize](#tileSize) | The size of a single file. |
| [textureSource](#textureSource) | The path to the used texture. |

## Functions

| Name | Description |
| ---- | ----------- |
| [TileSet](#TileSet) | Constructor that takes the path to the .rtiles file  |
| [TileSet](#TileSet) | Constructor that takes the path of the texture (image file) and a Vector2 that represents the tile size  |
| [TileSet](#TileSet) | Like TileSet(std::string textureSource, Vector2 tileSize), but takes an int for tile size with identical width and height (square tiles)  |
| [TileSet](#TileSet) | Constructor that takes the TileSetBin binary structure  |
| [dumpJson](#dumpJson) | Dump the TileSet's data into a nlohmann::json object  |
| [unload](#unload) | Unload routine  |
| [getTileSize](#getTileSize) | Get the size of a tile. |
| [setTileSize](#setTileSize) | Set the tile size to an integer value. |
| [setTileSizeVector](#setTileSizeVector) | Set the tile size to a Vector2 value. |
| [getTexture](#getTexture) | Get the texture of this TileSet. |
| [getTextureSource](#getTextureSource) | Get the path to the texture image file. |
| [setTextureSource](#setTextureSource) | Set the TileSet's texture source image. |
| [areAtlasCoordsValid](#areAtlasCoordsValid) | Check whether the given coordinates are valid  |
| [getTile](#getTile) | Get a tile from this TileSet as an AtlasTile. |

## Variable Details

### texture<a name="texture"></a>

!!! variable "Texture texture"

    The used Texture for this TileSet. A raylib Texture is used.
    

### textureSource<a name="textureSource"></a>

!!! variable "std::string textureSource"

    The path to the used texture. May be a .png file or other image format.
    

### tileSize<a name="tileSize"></a>

!!! variable "Vector2 tileSize"

    The size of a single file. TIles can have different widths and heights, thus a Vector2 is used.
    

## Function Details

### TileSet<a name="TileSet"></a>
!!! function "TileSet(std::string fileName)"

    Constructor that takes the path to the .rtiles file
    

!!! function "TileSet(std::string textureSource, Vector2 tileSize)"

    Constructor that takes the path of the texture (image file) and a Vector2 that represents the tile size
    

!!! function "TileSet(std::string textureSource, int tileSizeInt)"

    Like TileSet(std::string textureSource, Vector2 tileSize), but takes an int for tile size with identical width and height (square tiles)
    

!!! function "TileSet(TileSetBin bin)"

    Constructor that takes the TileSetBin binary structure
    

### areAtlasCoordsValid<a name="areAtlasCoordsValid"></a>
!!! function "bool areAtlasCoordsValid(Vector2 atlasCoords)"

    Check whether the given coordinates are valid
    

### dumpJson<a name="dumpJson"></a>
!!! function "json dumpJson()"

    Dump the TileSet's data into a nlohmann::json object
    

### getTexture<a name="getTexture"></a>
!!! function "Texture getTexture()"

    Get the texture of this TileSet.
    
    :material-location-exit: **Return**
    :    A raylib Texture.
    

### getTextureSource<a name="getTextureSource"></a>
!!! function "std::string getTextureSource()"

    Get the path to the texture image file.
    

### getTile<a name="getTile"></a>
!!! function "AtlasTile getTile(Vector2 atlasCoords)"

    Get a tile from this TileSet as an AtlasTile.
    

### getTileSize<a name="getTileSize"></a>
!!! function "Vector2 getTileSize()"

    Get the size of a tile. Since a tile may have different width and height, it returns a Vector2
    

### setTextureSource<a name="setTextureSource"></a>
!!! function "void setTextureSource(std::string source)"

    Set the TileSet's texture source image.
    

### setTileSize<a name="setTileSize"></a>
!!! function "void setTileSize(int size)"

    Set the tile size to an integer value. Both width and height will be this value.
    

### setTileSizeVector<a name="setTileSizeVector"></a>
!!! function "void setTileSizeVector(Vector2 vec)"

    Set the tile size to a Vector2 value. The x component is taken for width; the y component is taken for height.
    

### unload<a name="unload"></a>
!!! function "void unload()"

    Unload routine
    

