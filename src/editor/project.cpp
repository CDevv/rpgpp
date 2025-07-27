#include "project.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include "tileset.hpp"
#include "tilemap.hpp"
#include "room.hpp"
using json = nlohmann::json;

Project::Project() {}

Project::Project(std::string filePath)
{
    this->projectPath = filePath;
    this->projectBasePath = GetDirectoryPath(filePath.c_str());
    bool success = ChangeDirectory(projectBasePath.c_str());
    if (!success) {
        throw std::runtime_error("Changing working directory via raylib failed..");
    }

    char* jsonString = LoadFileText(filePath.c_str());
    json projectJson = json::parse(jsonString);

    std::string projectTitle = projectJson.at("title");
    this->projectTitle = projectTitle;
    std::string tileSetsPath = projectJson.at("tilesets");
    this->tileSetsPath = tileSetsPath;
    std::string mapsPath = projectJson.at("maps");
    this->mapsPath = mapsPath;

    this->tileSetPathsList = makeTileSetPaths();
    this->mapPathsList = makeMapPaths();

    UnloadFileText(jsonString);
}

GameData Project::generateStruct()
{
    GameData struc;
    struc.title = this->projectTitle;

    //TileSets
    for (auto tileSetPath : this->tileSetPathsList) {
        TileSet tileSet(tileSetPath);
        Texture texture = tileSet.getTexture();
        Image image = LoadImageFromTexture(texture);

        int fileSize = 0;
        std::string fileType = GetFileExtension(tileSet.getTextureSource().c_str());
        unsigned char* imageData = ExportImageToMemory(image, fileType.c_str(), &fileSize);

        TileSetBin tileSetBin;
        tileSetBin.name = GetFileName(tileSetPath.c_str());
        tileSetBin.extension = GetFileExtension(tileSet.getTextureSource().c_str());
        for (int i = 0; i < fileSize; i++) {
            tileSetBin.image.push_back(*imageData);
            imageData++;
        }
        tileSetBin.tileSize = tileSet.getTileSize();
        tileSetBin.dataSize = fileSize;

        struc.tilesets[GetFileName(tileSetPath.c_str())] = tileSetBin;

        UnloadImage(image);
    }

    for (auto roomPath : this->mapPathsList) {
        std::unique_ptr<TileMap> map = std::make_unique<TileMap>(roomPath);
        
        RoomBin roomBin;
        roomBin.name = GetFileName(roomPath.c_str());
        roomBin.tileSetName = GetFileName(map->getTileSetSource().c_str());
        Vector2 worldSize = map->getMaxWorldSize();
        roomBin.width = static_cast<int>(worldSize.x);
        roomBin.height = static_cast<int>(worldSize.y);

        for (int x = 0; x < roomBin.width; x++) {
            std::vector<TileBin> row;
            for (int y = 0; y < roomBin.height; y++) {
                TileBin tile;
                row.push_back(tile);   
            }
            roomBin.tiles.push_back(row);
        }
        for (int x = 0; x < roomBin.width; x++) {
            for (int y = 0; y < roomBin.height; y++) {
                Tile tile = map->getTile(x, y);
                Vector2 atlasPos = tile.getAtlasTile().getAtlasCoords();
                Vector2 worldPos = tile.getWorldCoords();

                IVector intAtlas = IVector {
                    static_cast<int>(atlasPos.x), static_cast<int>(atlasPos.y)
                };
                IVector intWorld = IVector {
                    static_cast<int>(worldPos.x), static_cast<int>(worldPos.y)
                };

                TileBin tileBin = TileBin { intAtlas, intWorld };
                roomBin.tiles[x][y] = tileBin;
            }
        }

        map.reset();

        std::unique_ptr<Room> room = std::make_unique<Room>(roomPath);
        for (auto collisionVec : room->getCollisionTiles()) {
            IVector intVec;
            intVec.x = static_cast<int>(collisionVec.x);
            intVec.y = static_cast<int>(collisionVec.y);
            roomBin.collisions.push_back(intVec);
        }
        for (auto interactable : room->getInteractableTiles()) {
            InteractableBin intBin;
            intBin.x = static_cast<int>(interactable.getRect().x);
            intBin.y = static_cast<int>(interactable.getRect().y);
            intBin.type = static_cast<int>(interactable.getType());
            roomBin.interactables.push_back(intBin);
        }
        room.reset();

        struc.rooms.push_back(roomBin);
    }

    return struc;
}

std::string Project::getProjectTitle()
{
    return projectTitle;
}

std::string Project::getProjectBasePath()
{
    return projectBasePath;
}

std::vector<std::string> Project::makeTileSetPaths()
{
    FilePathList pathList = LoadDirectoryFiles(tileSetsPath.c_str());

    auto vec = std::vector<std::string>();
    for (int i = 0; i < pathList.count; i++) {
        std::string pathStr = pathList.paths[i];
        vec.push_back(pathStr);
    }

    return vec;
}

std::vector<std::string> Project::makeMapPaths()
{
    FilePathList pathList = LoadDirectoryFiles(mapsPath.c_str());

    auto vec = std::vector<std::string>();
    for (int i = 0; i < pathList.count; i++) {
        std::string pathStr = pathList.paths[i];
        vec.push_back(pathStr);
    }

    return vec;
}

std::vector<std::string> Project::getTileSetPaths()
{
    return tileSetPathsList;
}

std::vector<std::string> Project::getMapPaths()
{
    return mapPathsList;
}