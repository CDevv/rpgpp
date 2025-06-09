#ifndef _RPGPP_GAME_H
#define _RPGPP_GAME_H

#include "stateService.hpp"
#include "worldService.hpp"
#include "interfaceService.hpp"

class Game {
private:
    static Game *instance_;
    static StateService *state;
    static WorldService *world;
    static InterfaceService *ui;
public:
    Game();
    static Game& instance();
    static StateService *getState();
    static WorldService *getWorld();
    static InterfaceService *getUi();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
