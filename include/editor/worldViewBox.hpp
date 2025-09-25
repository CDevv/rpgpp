#ifndef _RPGPP_EDITOR_WORLDVIEWBOX_H
#define _RPGPP_EDITOR_WORLDVIEWBOX_H

#include <memory>
#include <raylib.h>
#include "fileSystemService.hpp"
#include "mouseInput.hpp"
#include "room.hpp"
#include "tileset.hpp"

class WorldViewBox;

enum RoomAction {
    ACTION_NONE, ACTION_PLACE, ACTION_ERASE, ACTION_EDIT
};

enum RoomLayer {
    LAYER_TILES, LAYER_COLLISIONS, LAYER_INTERACTABLES
};

enum ViewBoxLayer {
    VIEWBOX_LAYER_BASE = 0, VIEWBOX_LAYER_BINVIEW = 1, VIEWBOX_LAYER_DIALOG = 2, VIEWBOX_LAYER_WINDOW = 3
};

class TileSetViewBox {
private:
    TileSet *tileSet;
    ViewBoxLayer layer;
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
    TileSetViewBox(WorldViewBox* viewBox, ViewBoxLayer boxLayer);
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
    ViewBoxLayer layer;
    Vector2 tileAtlasPos;
    Vector2 tileWorldPos;
    bool hoverValidTile;
    Vector2 selectedWorldTile;
    RoomAction action;
    RoomLayer currentLayer;
    bool isTileValid;
    Vector2 selectedTileAtlasCoords;
    InteractableType currentInteractableType;
    void drawHoverTile(int atlasTileSize, Vector2 tileWorldPos);
public:
    MapViewBox();
    MapViewBox(WorldViewBox *viewBox, ViewBoxLayer boxLayer);
    void unload();
    void setMap(Room* map);
    void setActionMode(RoomAction mode);
    void setLayerMode(RoomLayer mode);
    void setSelectedTile(Vector2 tile);
    void setCurrentInteractableType(InteractableType type);
    Vector2 getSelectedWorldTile();
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
    bool mouseLock;
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
    WorldViewBox(Rectangle windowRect, EngineFileType type, ViewBoxLayer boxLayer);
    ~WorldViewBox();
    Rectangle getWindowRect();
    void setRect(Rectangle rect);
    Rectangle getRenderRect();
    void setMouseLock(bool value);
    void setTileSet(TileSet* tileSet);
    void setRoom(Room* map);
    void enableTileSelection();
    Vector2 getSelectedTile();
    void setActionMode(RoomAction mode);
    void setLayerMode(RoomLayer mode);
    void setCurrentInteractableType(InteractableType type);
    void setSelectedTile(Vector2 tile);
    Vector2 getSelectedWorldTile();
    void update();
    void draw();
    void unload();
};

#endif
