#include "projectGenerator.hpp"
#include "dialogueBalloon.hpp"
#include "dialogueParser.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"
#include "room.hpp"
#include "actor.hpp"
#include <nlohmann/json.hpp>
#include <raylib.h>
using json = nlohmann::json;

ProjectGenerator::ProjectGenerator() {}

void ProjectGenerator::generateNewProj(std::string title, std::string path)
{
	std::string newDirPath = TextFormat("%s/%s", path.c_str(), title.c_str());
    int dirSuccess = MakeDirectory(newDirPath.c_str());

    if (dirSuccess != 0) {
        printf("Error when making a directory!\n");
        return;
    }

    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "images"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "tilesets"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "maps"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "actors"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "dialogues"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "images"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "fonts"));
    MakeDirectory(TextFormat("%s/%s", newDirPath.c_str(), "sounds"));

    json projectJson = {
        {"title", title},
        {"tilesets", "tilesets"},
        {"maps", "maps"},
        {"actors", "actors"},
        {"dialogues", "dialogues"},
        {"images", "images"},
        {"fonts", "fonts"}
    };
    std::string jsonString = projectJson.dump(4);

    SaveFileText(TextFormat("%s/%s", newDirPath.c_str(), "proj.rpgpp"), const_cast<char*>(jsonString.data()));
}

GameData ProjectGenerator::generateStruct(ProjectPaths proj, std::string title)
{
	GameData struc;
    struc.title = title;

    //TileSets
    for (auto tileSetPath : proj.tileSetPathsList) {
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
        tileSetBin.tileSize = IVector { static_cast<int>(tileSet.getTileSize().x), static_cast<int>(tileSet.getTileSize().y) };
        tileSetBin.dataSize = fileSize;

        struc.tilesets[GetFileName(tileSetPath.c_str())] = tileSetBin;

        UnloadImage(image);
    }

    for (auto roomPath : proj.mapPathsList) {
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

                TileBin tileBin;
                tileBin.atlasPos = intAtlas;
                tileBin.worldPos = intWorld;
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
        for (auto interactable : room->getInteractables().getList()) {
            InteractableBin intBin;
            intBin.x = static_cast<int>(interactable->pos.x);
            intBin.y = static_cast<int>(interactable->pos.y);
            intBin.type = static_cast<int>(interactable->type);
            intBin.onTouch = interactable->onTouch;

            switch (interactable->type) {
            case INT_TWO:
                intBin.dialogue = (static_cast<IntBase<DiagInt>*>(interactable))->get().dialogueSource;
                break;
            default:
                break;
            }
            roomBin.interactables.push_back(intBin);
        }
        room.reset();

        struc.rooms.push_back(roomBin);
    }

    //Actors
    for (auto actorPath : proj.actorPathsList) {
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(actorPath);

        ActorBin actorBin;
        actorBin.name = GetFileName(actorPath.c_str());
        actorBin.tileSetName = GetFileName(actor->getTileSetSource().c_str());

        Rectangle collisionRect = actor->getCollisionRect();
        actorBin.collision = IRect
        {
            static_cast<int>(collisionRect.x), static_cast<int>(collisionRect.y),
            static_cast<int>(collisionRect.width), static_cast<int>(collisionRect.height)
        };
        std::array<std::vector<Vector2>, 8> animations = actor->getAnimationsRaw();
        for (int i = 0; i < 8; i++) {
            for (int frameIndex = 0; frameIndex < animations[i].size(); frameIndex++) {
                Vector2 vec = animations[i][frameIndex];
                IVector intVec = IVector
                {
                    static_cast<int>(vec.x), static_cast<int>(vec.y)
                };

                actorBin.animations[i].push_back(intVec);
            }
        }

        struc.actors.push_back(actorBin);
    }

    //Dialogues
    for (auto diagPath : proj.dialoguesPathsList) {
        std::string diagText = LoadFileText(diagPath.c_str());
        Dialogue diag = parseDialogueText(diagText);

        diag.title = GetFileNameWithoutExt(diagPath.c_str());
        struc.dialogues[GetFileNameWithoutExt(diagPath.c_str())] = diag;
    }

    //Images
    for (auto imagePath : proj.imagesPathsList) {
        Image img = LoadImage(imagePath.c_str());
        ImageBin bin;

        int fileSize = 0;

        unsigned char* data = ExportImageToMemory(img, GetFileExtension(imagePath.c_str()), &fileSize);
        for (int i = 0; i < fileSize; i++) {
            bin.data.push_back(*data);
            data++;
        }
        bin.dataSize = fileSize;

        UnloadImage(img);

        struc.images[GetFileName(imagePath.c_str())] = bin;
    }

    //Fonts
    for (auto fontPath : proj.fontsPathsList) {

    }

    return struc;
}
