#ifndef _RPGPP_EDITOR_WORLDVIEWBOX_H
#define _RPGPP_EDITOR_WORLDVIEWBOX_H

#include <memory>
#include <raylib.h>
#include "mouseInput.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"

class WorldViewBox;

class TileSetViewBox {
private:
    TileSet *tileSet;
    WorldViewBox* viewBox;
    Vector2 tileAtlasPos;
    Vector2 tileWorldPos;
    bool hoverValidTile;
public:
    TileSetViewBox();
    TileSetViewBox(WorldViewBox* viewBox);
    void isHoverOnValidTile();
    void drawGrid();
    void drawTiles();
    void drawMouse();
};

class MapViewBox {
private:
    TileMap *map;
    WorldViewBox *viewBox;
    Vector2 tileAtlasPos;
    Vector2 tileWorldPos;
    bool hoverValidTile;
public:
    MapViewBox();
    MapViewBox(WorldViewBox *viewBox);
    void isHoverOnValidTile();
    void drawGrid();
    void drawTiles();
    void drawMouse();
};

class WorldViewBox {
    friend class TileSetViewBox;
    friend class MapViewBox;
private:
    Rectangle windowRect;
    Rectangle renderRect;
    Camera2D camera;
    std::unique_ptr<MouseInputComponent> mouseInput;
    TileSet *tileSet;
    RenderTexture renderTexture;
    std::string windowTitle;
    Vector2 mousePos;
    Vector2 hoverPos;
    TileSetViewBox tilesView;
    MapViewBox mapView;
public:
    WorldViewBox();
    WorldViewBox(Rectangle windowRect, Rectangle renderRect);
    void update();
    void draw();
    void unload();
};

#endif
