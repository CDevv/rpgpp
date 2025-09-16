#ifndef _RPGPP_EDITOR_MAINVIEW_H
#define _RPGPP_EDITOR_MAINVIEW_H

#include <raylib.h>
#include "mainViewPanel.hpp"

#include "textArea.hpp"

class MainView {
private:
	MainViewPanel panel;
	TextArea area;
public:
	MainView();
	void update();
	void draw();
};

#endif