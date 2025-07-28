#ifndef _RPGPP_EDITOR_PROJECTBINARYVIEWWINDOW_H
#define _RPGPP_EDITOR_PROJECTBINARYVIEWWINDOW_H

#include "gamedata.hpp"
#include "fileSystemService.hpp"
#include <raylib.h>
#include <memory>
#include "tileset.hpp"
#include "room.hpp"
#include "worldViewBox.hpp"

class ProjectBinaryViewWindow {
private:
	Rectangle rect;
	std::unique_ptr<WorldViewBox> tilesView;
	std::unique_ptr<WorldViewBox> roomView;
	bool active;
	bool dataAvailable;
	bool pageEditMode;
	int currentPageNum;
	EngineFileType currentPage;
	std::unique_ptr<GameData> data;
	EngineFileType fileType;
	std::unique_ptr<TileSet> tileset;
	std::unique_ptr<Room> room;
	void setTileSet(std::string name);
	void setRoom(RoomBin roomBin);
	void drawResourcesList();
public:
	ProjectBinaryViewWindow();
	ProjectBinaryViewWindow(Rectangle rect);
	void setActive();
    void closeWindow();
	void draw();
	void setData(GameData data);
};

#endif	
