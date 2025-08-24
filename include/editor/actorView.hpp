#ifndef _RPGPP_EDITOR_ACTORVIEW_H
#define _RPGPP_EDITOR_ACTORVIEW_H

#include <memory>
#include <raylib.h>
#include "mouseInput.hpp"
#include "collisionbox.hpp"

class ActorView {
private:
	Rectangle rect;
	Rectangle renderRect;
	RenderTexture2D renderTexture;
	Camera2D camera;
	Vector2 mouseWorldPos;
	CollisionBox collisionBox;
	bool collisionViewActive;
	void drawActor();
	void drawCollision();
public:
	ActorView();
	ActorView(Rectangle rect);
	void setInitial();
	void update();
	void draw();
	void setCollisionActive(bool value);
};

#endif