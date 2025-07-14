#ifndef _RPGPP_EDITOR_WORLDVIEWBOX_H
#define _RPGPP_EDITOR_WORLDVIEWBOX_H

#include <memory>
#include <raylib.h>
#include "fileSystemService.hpp"
#include "mouseInput.hpp"
#include "room.hpp"
#include "tileset.hpp"

class WorldViewBox;

class TileSetViewBox {
private:
    TileSet *tileSet;
    WorldViewBox* viewBox;
    Vector2 tileAtlasPos;
    Vector2 tileWorldPos;
    bool hoverValidTile;
    bool isSelectingTile;
    bool hasSelectedTile;
    Vector2 selectedTileAtlasPos;
    Vector2 selectedTileWorldPos;
public:
    TileSetViewBox();
    TileSetViewBox(WorldViewBox* viewBox);
    void unload();
    void setTileSet(TileSet* tileSet);
    void setSelectionMode();
    Vector2 getSelectedTile();
    void isHoverOnValidTile();
    void drawGrid();
    void drawTiles();
    void drawMouse();
};

class MapViewBox {
private:
    Room *room;
    WorldViewBox *viewBox;
    Vector2 tileAtlasPos;
    Vector2 tileWorldPos;
    bool hoverValidTile;
    bool isPlacing;
    bool isTileValid;
    Vector2 selectedTileAtlasCoords;
public:
    MapViewBox();
    MapViewBox(WorldViewBox *viewBox);
    void unload();
    void setMap(Room* map);
    void setPlacingMode();
    void setSelectedTile(Vector2 tile);
    void isHoverOnValidTile();
    void drawGrid();
    void drawTiles();
    void drawMouse();
};

class WorldViewBox {
    friend class TileSetViewBox;
    friend class MapViewBox;
private:
    std::string windowTitle;
    Rectangle windowRect;
    Rectangle renderRect;
    Camera2D camera;
    RenderTexture renderTexture;
    EngineFileType type;
    std::unique_ptr<MouseInputComponent> mouseInput;
    Vector2 mousePos;
    Vector2 hoverPos;
    TileSetViewBox tilesView;
    MapViewBox mapView;
public:
    WorldViewBox();
    WorldViewBox(Rectangle windowRect, Rectangle renderRect, EngineFileType type);
    ~WorldViewBox();
    void setTileSet(TileSet* tileSet);
    void setRoom(Room* map);
    void enableTileSelection();
    Vector2 getSelectedTile();
    void enableTilePlacement();
    void setSelectedTile(Vector2 tile);
    void update();
    void draw();
    void unload();
};

#endif
