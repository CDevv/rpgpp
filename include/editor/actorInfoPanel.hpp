#ifndef _RPGPP_EDITOR_ACTORINFOPANEL_H
#define _RPGPP_EDITOR_ACTORINFOPANEL_H

#include "worldViewBox.hpp"
#include <raylib.h>

class ActorInfoPanel {
private:
    Rectangle rect;
    RoomAction action;
    std::string actorConcat;
    int currentActor;
public:
    ActorInfoPanel();
    ActorInfoPanel(Rectangle rect);
    void setRect(Rectangle rect);
    void setActionMode(RoomAction action);
    int getCurrentActorIndex();
    void update();
    void draw();
};

#endif
