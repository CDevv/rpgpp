#ifndef _RPGPP_EDITOR_PROJECTBINARYVIEWWINDOW_H
#define _RPGPP_EDITOR_PROJECTBINARYVIEWWINDOW_H

#include "gamedata.hpp"
#include <raylib.h>
#include <memory>

class ProjectBinaryViewWindow {
private:
	Rectangle rect;
	bool active;
	bool dataAvailable;
	std::unique_ptr<GameData> data;
public:
	ProjectBinaryViewWindow();
	ProjectBinaryViewWindow(Rectangle rect);
	void setActive();
    void closeWindow();
	void draw();
	void setData(GameData data);
};

#endif	
