#ifndef _RPGPP_EDITOR_ACTORPANELVIEW_H
#define _RPGPP_EDITOR_ACTORPANELVIEW_H

#include <raylib.h>

class ActorPanelView {
private:
	Rectangle rect;
public:
	ActorPanelView();
	ActorPanelView(Rectangle rect);
	void setInitial();
	void update();
	void draw();
};

#endif