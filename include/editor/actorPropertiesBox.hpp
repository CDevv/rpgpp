#ifndef _RPGPP_EDITOR_ACTORPROPERTIESBOX_H
#define _RPGPP_EDITOR_ACTORPROPERTIESBOX_H

#include "actor.hpp"
#include <raylib.h>

class ActorPropertiesBox {
private:
	Rectangle rect;
	Vector2 scrollVec;
    Rectangle viewRec;
    Actor* actor;
    bool collisionViewActive;
public:
	ActorPropertiesBox();
	ActorPropertiesBox(Rectangle rect);
	void setDefaults();
	void update();
	void draw();
	bool getCollisionViewActive();
};

#endif