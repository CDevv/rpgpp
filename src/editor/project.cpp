#include "project.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include "tileset.hpp"
#include "tilemap.hpp"
#include "room.hpp"
using json = nlohmann::json;

#ifdef _WIN32
#include "winapi.hpp"
#endif

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
    std::string actorsPath = projectJson.at("actors");
    this->actorsPath = actorsPath;

    this->tileSetPathsList = makeTileSetPaths();
    this->mapPathsList = makeMapPaths();
    this->actorPathsList = makeActorPaths();

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
        for (auto interactable : room->getInteractableTiles()) {
            InteractableBin intBin;
            intBin.x = static_cast<int>(interactable.getRect().x / 48);
            intBin.y = static_cast<int>(interactable.getRect().y / 48);
            intBin.type = static_cast<int>(interactable.getType());
            roomBin.interactables.push_back(intBin);
        }
        room.reset();

        struc.rooms.push_back(roomBin);
    }

    for (auto actorPath : this->actorPathsList) {
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

    return struc;
}

void Project::generateCmdScript()
{
    #ifdef _WIN32
    //Generate a .cmd file on Windows
    VsInfo vsInfo = WinVsWhere(std::string(GetApplicationDirectory()).append("vswhere.exe"));
    printf("%s \n", vsInfo.auxiliaryPath.c_str());

    std::string scriptSource;
    std::string callVcBat = "call ";
    callVcBat.append("\"").append(vsInfo.auxiliaryPath).append("\\vcvars64.bat").append("\"");

    std::string clLine = "cl /nologo /c /EHs /I ";
    clLine.append("\"").append(GetApplicationDirectory()).append("\include").append("\"")
    .append(" /I ./include /I \"").append(GetApplicationDirectory()).append("game-src\\include\" main.cpp");

    std::string appDir = std::string(GetApplicationDirectory()).substr(0, TextLength(GetApplicationDirectory()) - 1);

    std::string exeName = TextFormat("%s.exe", projectTitle.c_str());
    std::string linkLine = "link ";
    linkLine.append("/NOLOGO /LIBPATH:\"").append(GetApplicationDirectory()).append("\lib").append("\"")
    .append(" /LIBPATH:").append("\"").append(appDir).append("\"")
    .append(" /LIBPATH:\"").append(appDir).append("\\game-src\\lib\"")
    .append(" /OUT:").append(exeName)
    .append(" raylib.lib rpgpp.lib opengl32.lib gdi32.lib shell32.lib user32.lib winmm.lib")
    .append(" main.obj");

    scriptSource.append(callVcBat).append("\n\n").append(clLine).append("\n").append(linkLine).append("\n");
    printf("%s \n", scriptSource.c_str());

    SaveFileText("build.cmd", const_cast<char*>(scriptSource.data()));
    #endif
}

void Project::callCompiler()
{
    std::string cppCode = R"(#include <raylib.h>
#include "game.hpp"

int main()
{
    InitWindow(640, 480, "RPG++ Game.");

    Game game;
    game.init();

    Game::useBin("game.bin");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.draw();
        EndDrawing();
    }

    game.unload();
    CloseWindow();

    return 0;
}
    )";
    SaveFileText("main.cpp", const_cast<char*>(cppCode.data()));

    #ifdef _WIN32
    std::string cmdLine = TextFormat("C:\\Windows\\System32\\cmd.exe /c \"%s\"", "build.cmd");
    WinCreateProc(cmdLine);
    #endif
}

void Project::cleanCompilation()
{
    #ifdef _WIN32
    std::filesystem::remove(std::string(TextFormat("%s\\build.cmd", projectBasePath.c_str())));
    std::filesystem::remove(std::string(TextFormat("%s\\main.cpp", projectBasePath.c_str())));
    std::filesystem::remove(std::string(TextFormat("%s\\main.obj", projectBasePath.c_str())));
    #endif
}

void Project::compileProject()
{
    //Save binary file
    std::string binFile = std::string(this->getProjectBasePath()).append("/game.bin");
    serializeDataToFile(binFile, this->generateStruct());
    printf("CompileProject: Saved game.bin\n");

    //Generate a shell/cmd script
    this->generateCmdScript();
    printf("CompileProject: Saved command batch script\n");

    //Call the batch script
    this->callCompiler();
    printf("CompileProject: Compilation is done\n");

    //Clean up
    this->cleanCompilation();
    printf("CompileProject: Cleaned up\n");

    #ifdef _WIN32
    printf("The executable is located at: %s\\%s.exe", projectBasePath.c_str(), projectTitle.c_str());
    #endif
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

std::vector<std::string> Project::makeActorPaths()
{
    FilePathList pathList = LoadDirectoryFiles(actorsPath.c_str());

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

std::vector<std::string> Project::getActorPaths()
{
    return actorPathsList;
}