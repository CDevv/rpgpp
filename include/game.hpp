#ifndef _RPGPP_GAME_H
#define _RPGPP_GAME_H

#include "stateService.hpp"

class Game {
private:
    static Game *instance_;
    static StateService *state;
public:
    Game();
    static Game& instance();
    static StateService *getState();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
