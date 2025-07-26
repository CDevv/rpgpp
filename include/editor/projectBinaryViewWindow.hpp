#ifndef _RPGPP_EDITOR_PROJECTBINARYVIEWWINDOW_H
#define _RPGPP_EDITOR_PROJECTBINARYVIEWWINDOW_H

#include "gamedata.hpp"
#include <raylib.h>
#include <memory>
#include "tileset.hpp"
#include "worldViewBox.hpp"

class ProjectBinaryViewWindow {
private:
	Rectangle rect;
	std::unique_ptr<WorldViewBox> tilesView;
	bool active;
	bool dataAvailable;
	std::unique_ptr<GameData> data;
	std::unique_ptr<TileSet> tileset;
public:
	ProjectBinaryViewWindow();
	ProjectBinaryViewWindow(Rectangle rect);
	void setActive();
    void closeWindow();
	void draw();
	void setData(GameData data);
};

#endif	
