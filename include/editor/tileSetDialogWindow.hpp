#ifndef _RPGPP_EDITOR_TILESETDIALOGWINDOW_H
#define _RPGPP_EDITOR_TILESETDIALOGWINDOW_H

#include <raylib.h>
#include "worldViewBox.hpp"

class TileSetDialogWindow {
private:
	bool active;
	ViewBoxLayer boxLayer;
	Rectangle rect;
	Vector2 chosenTile;
public:
	TileSetDialogWindow();
	TileSetDialogWindow(Rectangle rect);
	void setActive();
	void closeWindow();
	void update();
	void draw();
};

#endif