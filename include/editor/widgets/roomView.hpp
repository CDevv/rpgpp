#ifndef _RPGPP_ROOMVIEW_H
#define _RPGPP_ROOMVIEW_H

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
	IVector getTileAtMouse();
	Rectangle getSourceRect(TileMap *tileMap, int x, int y);
	Rectangle getDestRect(TileMap *tileMap, int x, int y);
	void handlePlaceMode(int x, int y);

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
};

#endif