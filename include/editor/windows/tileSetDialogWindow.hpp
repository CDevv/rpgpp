#ifndef _RPGPP_EDITOR_TILESETDIALOGWINDOW_H
#define _RPGPP_EDITOR_TILESETDIALOGWINDOW_H

#include <raylib.h>
#include <memory>
#include <functional>
#include "worldViewBox.hpp"

class TileSetDialogWindow {
private:
	bool active;
	ViewBoxLayer boxLayer;
	Rectangle rect;
	Vector2 chosenTile;
	std::unique_ptr<WorldViewBox> tileSetView;
	std::function<void(Vector2)> callback;
public:
	TileSetDialogWindow();
	TileSetDialogWindow(Rectangle rect);
	void setActive();
	void closeWindow();
	void update();
	void draw();
	void setTileSet(TileSet* tileSet);
	void setSelectedTile(Vector2 atlasTile);
	Vector2 getSelectedTile();
	void setCallback(const std::function<void(Vector2)> &func);
};

#endif