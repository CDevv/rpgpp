#ifndef _RPGPP_EDITOR_MAPTOOLSBOX_H
#define _RPGPP_EDITOR_MAPTOOLSBOX_H

#include "worldViewBox.hpp"
#include <raylib.h>

class MapToolsBox {
private:
	Rectangle rect;
	int actionInt;
	RoomAction action;
public:
	MapToolsBox();
	MapToolsBox(Rectangle rect);
	RoomAction getActionMode();
	void update();
	void draw();
};

#endif