#ifndef _RPGPP_EDITOR_ACTORPANELVIEW_H
#define _RPGPP_EDITOR_ACTORPANELVIEW_H

#include <raylib.h>
#include "actorView.hpp"
#include "propertiesBox.hpp"

class ActorPanelView {
private:
	Rectangle rect;
	ActorView actorView;
	PropertiesBox props;
public:
	ActorPanelView();
	ActorPanelView(Rectangle rect);
	void setInitial();
	void update();
	void draw();
};

#endif