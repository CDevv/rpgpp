#ifndef _RPGPP_EDITOR_ACTORVIEW_H
#define _RPGPP_EDITOR_ACTORVIEW_H

#include <memory>
#include <vector>
#include <raylib.h>
#include "mouseInput.hpp"
#include "collisionbox.hpp"

class ActorView {
private:
	int frameCounter;
	Rectangle rect;
	Rectangle renderRect;
	RenderTexture2D renderTexture;
	Camera2D camera;
	Vector2 mouseWorldPos;
	CollisionBox collisionBox;
	bool collisionViewActive;
	int currentAnim;
	int currentFrame;
	std::vector<Vector2> currentAnimFrames;
	bool animPlaying;
	void drawActor();
public:
	ActorView();
	ActorView(Rectangle rect);
	void setInitial();
	void updateData();
	void update();
	void draw();
	void setCollisionActive(bool value);
	void setAnimation(int id);
	void setAnimPlaying(bool value);
	void setFrame(int frame);
	int getFrame();
};

#endif