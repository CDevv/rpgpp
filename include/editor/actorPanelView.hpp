#ifndef _RPGPP_EDITOR_ACTORPANELVIEW_H
#define _RPGPP_EDITOR_ACTORPANELVIEW_H

#include <raylib.h>
#include "actorView.hpp"
#include "propertiesBox.hpp"
#include "animationsView.hpp"

class ActorPanelView {
private:
	Rectangle rect;
	ActorView actorView;
	PropertiesBox props;
	AnimationsView animationsView;
public:
	ActorPanelView();
	ActorPanelView(Rectangle rect);
	void setRect(Rectangle rect);
	void setInitial();
	void update();
	void draw();
};

#endif