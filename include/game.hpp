#ifndef _RPGPP_GAME_H
#define _RPGPP_GAME_H

class WorldService;

#include <memory>
#include <string>
#include "gamedata.hpp"
#include "stateService.hpp"
#include "worldService.hpp"
#include "interfaceService.hpp"

#define RPGPP_VER "0.1"

class Game {
private:
    static Game *instance_;
    static std::unique_ptr<GameData> gameData;
    static bool usesBin;
    static std::unique_ptr<StateService> state;
    static std::unique_ptr<WorldService> world;
    static std::unique_ptr<InterfaceService> ui;
public:
    Game();
    static Game& instance();
    static void useBin(std::string filePath);
    static GameData& getBin();
    static StateService& getState();
    static WorldService& getWorld();
    static InterfaceService& getUi();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
