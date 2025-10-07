#ifndef _RPGPP_EDITOR_ACTORINITWINDOW_H
#define _RPGPP_EDITOR_ACTORINITWINDOW_H

#include <raylib.h>
#include <string>

class ActorInitWindow {
private:
	Rectangle rect;
	bool active;
	bool titleEditMode;
	char title[256];
	std::string titleText;
	bool hasSetTileSet;
	std::string tileSetSource;
public:
	ActorInitWindow();
	ActorInitWindow(Rectangle rect);
	void closeWindow();
	void setActive();
	void draw();
};

#endif
