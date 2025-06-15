#ifndef _RPGPP_GAME_H
#define _RPGPP_GAME_H

#include <memory>
#include "stateService.hpp"
#include "worldService.hpp"
#include "interfaceService.hpp"

class Game {
private:
    static Game *instance_;
    static std::unique_ptr<StateService> state;
    static std::unique_ptr<WorldService> world;
    static std::unique_ptr<InterfaceService> ui;
public:
    Game();
    static Game& instance();
    static StateService& getState();
    static WorldService& getWorld();
    static InterfaceService& getUi();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
