---
generator: doxide
---


# TileMap

**class TileMap**

A TileMap represents a map of the placed tiles in a Room.


## Variables

| Name | Description |
| ---- | ----------- |
| [basePos](#basePos) | Base position of this TileMap. |
| [tileSetSource](#tileSetSource) | File path to the source of the used TileSet. |
| [tileSet](#tileSet) | A unique_ptr to the used TileSet. |
| [width](#width) | Width of the TileMap in tiles  |
| [height](#height) | Height of the TileMap in height  |
| [tiles](#tiles) | A collection representing the tiles in this map  |

## Functions

| Name | Description |
| ---- | ----------- |
| [TileMap](#TileMap) | Construct from a file name/path  |
| [TileMap](#TileMap) | Construct using a TileSet path, width, height  |
| [TileMap](#TileMap) | Constructor using a TileSet object  |
| [TileMap](#TileMap) | Constructor using a unique_ptr of a TileSet  |
| [TileMap](#TileMap) | Constructor using a RoomBin binary structure  |
| [dumpJson](#dumpJson) | Dump the TileMap's data into a nlohmann::json object  |
| [unload](#unload) | Unload routine. |
| [update](#update) | Update routine. |
| [draw](#draw) | Draw routine. |
| [getTileSetSource](#getTileSetSource) | Get the path of the TileSet resource  |
| [getTileSet](#getTileSet) | Get a pointer to the used TileSet  |
| [setTileSet](#setTileSet) | Set a new TileSet for this TileMap using a path for the new TileSet  |
| [getTile](#getTile) | Get the Tile at the specified x and y position  |
| [setTile](#setTile) | Set the tile at the specified world position to be a tile at the specified atlas position from the TileSet  |
| [setEmptyTile](#setEmptyTile) | Set the tile at the specified world position to be empty (not drawn)  |
| [drawTile](#drawTile) | Draw the tile at the specified world x and y coordinates  |
| [getMaxWorldSize](#getMaxWorldSize) | Get the size of this TileMap :material-location-exit: **Return** :    the size in a Vector2. The x component is the width; the y component is the height.  |
| [setWorldSize](#setWorldSize) | Set the size of this TileSet using the provided Vector2 argument. |

## Variable Details

### basePos<a name="basePos"></a>

!!! variable "Vector2 basePos"

    Base position of this TileMap.
    

### height<a name="height"></a>

!!! variable "int height"

    Height of the TileMap in height
    

### tileSet<a name="tileSet"></a>

!!! variable "std::unique_ptr&lt;TileSet&gt; tileSet"

    A unique_ptr to the used TileSet.
    

### tileSetSource<a name="tileSetSource"></a>

!!! variable "std::string tileSetSource"

    File path to the source of the used TileSet.
    

### tiles<a name="tiles"></a>

!!! variable "std::vector&lt;std::vector&lt;Tile&gt;&gt; tiles"

    A collection representing the tiles in this map
    

### width<a name="width"></a>

!!! variable "int width"

    Width of the TileMap in tiles
    

## Function Details

### TileMap<a name="TileMap"></a>
!!! function "TileMap(std::string fileName)"

    Construct from a file name/path
    

!!! function "TileMap(std::string tileSetSource, int width, int height, int atlasTileSize, int worldTileSize)"

    Construct using a TileSet path, width, height
    

!!! function "TileMap(TileSet tileSet, int width, int height, int atlasTileSize, int worldTileSize)"

    Constructor using a TileSet object
    

!!! function "TileMap(std::unique_ptr&lt;TileSet&gt; tileSetPtr, int width, int height, int atlasTileSize, int worldTileSize)"

    Constructor using a unique_ptr of a TileSet
    

!!! function "TileMap(RoomBin bin)"

    Constructor using a RoomBin binary structure
    

### draw<a name="draw"></a>
!!! function "void draw()"

    Draw routine.
    

### drawTile<a name="drawTile"></a>
!!! function "void drawTile(int x, int y)"

    Draw the tile at the specified world x and y coordinates
    

### dumpJson<a name="dumpJson"></a>
!!! function "json dumpJson()"

    Dump the TileMap's data into a nlohmann::json object
    

### getMaxWorldSize<a name="getMaxWorldSize"></a>
!!! function "Vector2 getMaxWorldSize()"

    Get the size of this TileMap
    
    :material-location-exit: **Return**
    :    the size in a Vector2. The x component is the width; the y component is the height.
    

### getTile<a name="getTile"></a>
!!! function "Tile getTile(int x, int y)"

    Get the Tile at the specified x and y position
    

### getTileSet<a name="getTileSet"></a>
!!! function "TileSet&#42; getTileSet()"

    Get a pointer to the used TileSet
    

### getTileSetSource<a name="getTileSetSource"></a>
!!! function "std::string getTileSetSource()"

    Get the path of the TileSet resource
    

### setEmptyTile<a name="setEmptyTile"></a>
!!! function "void setEmptyTile(Vector2 worldPos)"

    Set the tile at the specified world position to be empty (not drawn)
    

### setTile<a name="setTile"></a>
!!! function "void setTile(Vector2 worldPos, Vector2 atlasPos)"

    Set the tile at the specified world position to be a tile at the specified atlas position from the TileSet
    

### setTileSet<a name="setTileSet"></a>
!!! function "void setTileSet(std::string tileSetSource)"

    Set a new TileSet for this TileMap using a path for the new TileSet
    

### setWorldSize<a name="setWorldSize"></a>
!!! function "void setWorldSize(Vector2 size)"

    Set the size of this TileSet using the provided Vector2 argument. The x ocmponent will be the new width; the y component will be the new height.
    

### unload<a name="unload"></a>
!!! function "void unload()"

    Unload routine. Typically calls raylib's UnloadTexture.
    

### update<a name="update"></a>
!!! function "void update()"

    Update routine.
    

