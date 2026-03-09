#ifndef _RPGPP_ROOMVIEWMODESHANDLER_H
#define _RPGPP_ROOMVIEWMODESHANDLER_H

#include "TGUI/Vector2.hpp"
#include "gamedata.hpp"
#include "room.hpp"
#include "views/roomView.hpp"
#include "views/tileSetView.hpp"
#include <memory>

struct RoomViewState {
	RoomTool tool;
	Room *room;
	TileSetView *tileSetView;
	Vector2 mouseWorldPos;
	IVector tileMouse;
	Rectangle destRect;
};

class RoomViewModesHandler {
  public:
	RoomViewState state;
	std::weak_ptr<RoomView> view;
	RoomViewModesHandler();

	void handleMode(int x, int y);
	void handlePlaceMode(int x, int y);
	void handleEraseMode(int x, int y);
	void handleEditMode(int x, int y);

	void handleModePress(tgui::Vector2f pos);
	void handlePlacePress(tgui::Vector2f pos);
	void handleErasePress(tgui::Vector2f pos);
	void handleEditPress(tgui::Vector2f pos);
};

#endif
