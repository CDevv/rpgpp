#ifndef _RPGPP_EDITOR_ACTORVIEW_H
#define _RPGPP_EDITOR_ACTORVIEW_H

#include <raylib.h>

class ActorView {
private:
	Rectangle rect;
	Rectangle renderRect;
	RenderTexture2D renderTexture;
	Camera2D camera;
	void drawActor();
public:
	ActorView();
	ActorView(Rectangle rect);
	void update();
	void draw();
};

#endif