#ifndef _RPGPP_EDITOR_WORLDVIEWBOX_H
#define _RPGPP_EDITOR_WORLDVIEWBOX_H

#include <memory>
#include <raylib.h>
#include "fileSystemService.hpp"
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
    void setTileSet(TileSet* tileSet);
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
    void setMap(TileMap* map);
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
    void setMap(TileMap* map);
    void update();
    void draw();
    void unload();
};

#endif
