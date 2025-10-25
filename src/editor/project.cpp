#include "project.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include "gamedata.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"
#include "room.hpp"
using json = nlohmann::json;

#ifdef _WIN32
#include "winapi.hpp"
#endif

#include <reproc++/run.hpp>

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

    this->projectTitle = projectJson.at("title");
    this->tileSetsPath = projectJson.at("tilesets");
    this->mapsPath = projectJson.at("maps");
    this->actorsPath = projectJson.at("actors");
    this->dialoguesPath = projectJson.at("dialogues");
    this->imagesPath = projectJson.at("images");
    this->fontsPath = projectJson.at("fonts");

    this->tileSetPathsList = makePaths(this->tileSetsPath);
    this->mapPathsList = makePaths(this->mapsPath);
    this->actorPathsList = makePaths(this->actorsPath);
    this->dialoguePathsList = makePaths(this->dialoguesPath);
    this->imagePathsList = makePaths(this->imagesPath);
    this->fontPathsList = makePaths(this->fontsPath);

    UnloadFileText(jsonString);
}

void Project::reloadPaths()
{
    this->tileSetPathsList = makePaths(this->tileSetsPath);
    this->mapPathsList = makePaths(this->mapsPath);
    this->actorPathsList = makePaths(this->actorsPath);
    this->dialoguePathsList = makePaths(this->dialoguesPath);
    this->imagePathsList = makePaths(this->imagesPath);
    this->fontPathsList = makePaths(this->fontsPath);
}

void Project::generateNewProj(std::string title, std::string path)
{
    ProjectGenerator::generateNewProj(title, path);
}

GameData Project::generateStruct()
{
    ProjectPaths paths;
    paths.tileSetPathsList = tileSetPathsList;
    paths.mapPathsList = mapPathsList;
    paths.actorPathsList = actorPathsList;
    paths.dialoguesPathsList = dialoguePathsList;
    paths.imagesPathsList = imagePathsList;
    paths.fontsPathsList = fontPathsList;
    return projectGenerator.generateStruct(paths, projectTitle);
}

void Project::generateCmdScript()
{
    projectCompiler.generateCmdScript(projectTitle);
}

void Project::callCompiler()
{
    projectCompiler.callCompiler();
}

void Project::cleanCompilation()
{
    projectCompiler.cleanCompilation(projectBasePath);
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
    #ifdef _WIN32
    this->cleanCompilation();
    printf("CompileProject: Cleaned up\n");
    printf("The executable is located at: %s\\%s.exe \n", projectBasePath.c_str(), projectTitle.c_str());
    #else
    printf("The executable is located at: %s/%s \n", projectBasePath.c_str(), projectTitle.c_str());
    #endif
}

void Project::runProject()
{
    //Save game.bin first
    std::string binFile = std::string(projectBasePath).append("/game.bin");
    GameData data =  this->generateStruct();
    serializeDataToFile(binFile, data);

    std::string luaCodeString = R"(
    printer()

    init_window(640, 480, "lraylib")

    g = game.new()
    g:init()

    game.use_bin("game.bin")

    set_fps(60)

    while not window_should_close() do
        g:update()
        begin_drawing()
        clear_background()
        g:draw()
        end_drawing()
    end

    close_window()
    )";
    SaveFileText("run.lua", const_cast<char*>(luaCodeString.data()));

    std::string fromLuaLib;
    std::string toLuaLib;

    #ifdef _WIN32
    fromLuaLib = TextFormat("%s\\game-src\\lib\\rpgpplua.dll", GetApplicationDirectory());
    toLuaLib = TextFormat("%s\\rpgpplua.dll", projectBasePath.c_str());
    #else
    fromLuaLib = TextFormat("%sgame-src/lib/librpgpplua.so", GetApplicationDirectory());
    toLuaLib = TextFormat("%s/rpgpplua.so", projectBasePath.c_str());
    #endif
    std::filesystem::copy(fromLuaLib, toLuaLib, std::filesystem::copy_options::update_existing);

    reproc::options options;
    options.redirect.parent = true;

    std::vector<std::string> rargs;
    #ifdef _WIN32
    rargs.push_back(std::string("\"").append(GetApplicationDirectory()).append("lua.exe").append("\""));
    #else
    rargs.push_back(std::string(GetApplicationDirectory()).append("lua"));
    #endif

    rargs.push_back("-lrpgpplua");

    #ifdef _WIN32
    rargs.push_back(std::string("\"").append(projectBasePath).append("\\").append("run.lua").append("\""));
    #else
    rargs.push_back(std::string(projectBasePath).append("/").append("run.lua"));
    #endif

    printf("%s \n", rargs[0].c_str());
    printf("%s \n", rargs[1].c_str());
    printf("%s \n", rargs[2].c_str());
    reproc::process p;
    std::error_code ec = p.start(rargs, options);
    if (ec) {
        printf("%s \n", ec.message().c_str());
    }
    printf("ended\n");

    #ifdef _WIN32
    std::string cmdLine = std::string(rargs[0]).append(" ").append(rargs[1]).append(" ").append(rargs[2]);
    printf("%s \n", cmdLine.c_str());
    WinCreateProc(cmdLine);

    std::filesystem::remove(toLuaLib);
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

std::vector<std::string> Project::makePaths(std::string dirPath)
{
    FilePathList pathList = LoadDirectoryFiles(dirPath.c_str());

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

std::vector<std::string> Project::getDialoguePaths()
{
    return dialoguePathsList;
}

std::vector<std::string> Project::getImagePaths()
{
    return imagePathsList;
}

std::vector<std::string> Project::getFontPaths()
{
    return fontPathsList;
}
