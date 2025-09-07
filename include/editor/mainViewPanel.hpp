#ifndef _RPGPP_EDITOR_MAINVIEWPANEL_H
#define _RPGPP_EDITOR_MAINVIEWPANEL_H

#include <raylib.h>

class MainViewPanel {
private:
	Rectangle rect;
	Texture logoTexture;
public:
	MainViewPanel();
	MainViewPanel(Rectangle rect);
	void update();
	void draw();
};

#endif