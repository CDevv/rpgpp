#ifndef _RPGPP_ROOMVIEW_H
#define _RPGPP_ROOMVIEW_H

#include "TGUI/Vector2.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tileSetView.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "worldView.hpp"
#include <memory>

class RoomView : public WorldView {
  private:
	IVector selectedTile{-1, -1};
	bool brushMode = false;
	IVector getTileAtMouse();
	Rectangle getSourceRect(TileMap *tileMap, int x, int y);
	Rectangle getDestRect(TileMap *tileMap, int x, int y);
	void handleMode(int x, int y);
	void handlePlaceMode(int x, int y);
	void handleEraseMode(int x, int y);
	void handleEditMode(int x, int y);
	void handleModePress(tgui::Vector2f pos);
	void handlePlacePress(tgui::Vector2f pos);
	void handleErasePress(tgui::Vector2f pos);
	void handleEditPress(tgui::Vector2f pos);

  public:
	typedef std::shared_ptr<RoomView> Ptr;

	Room *room{nullptr};
	TileSetView *tileSetView{nullptr};

	void drawCanvas() override;
	void drawOverlay() override;
	static RoomView::Ptr create();
	static RoomView::Ptr create(Room *room);
	void setRoom(Room *room);
	Room *getRoom();

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;

	void setBrush(bool value);
};

#endif