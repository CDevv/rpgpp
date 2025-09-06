#ifndef _RPGPP_EDITOR_MAINVIEW_H
#define _RPGPP_EDITOR_MAINVIEW_H

#include <raylib.h>
#include "mainViewPanel.hpp"

class MainView {
private:
	MainViewPanel panel;
public:
	MainView();
	void update();
	void draw();
};

#endif