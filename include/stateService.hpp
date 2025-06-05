#ifndef _RPGPP_STATESERVICE_H
#define _RPGPP_STATESERVICE_H

#include "room.hpp"

class StateService {
private:
    Room *room;
public:
    StateService();
    Room *getRoom();
    void unload();
};

#endif
