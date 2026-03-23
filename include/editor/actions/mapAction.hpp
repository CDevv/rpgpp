#ifndef _RPGPP_MAPACTION_H
#define _RPGPP_MAPACTION_H

#include "action.hpp"
#include "room.hpp"
#include "views/roomView.hpp"
#include <memory>
#include <string>
#include <utility>

struct MapActionData {
	RoomView *view;
	Room *room;
	RoomLayer layer;
	Vector2 worldTile;
	Vector2 tile;
	Vector2 prevTile;
	std::string interactable;
	std::string interactableFullPath;
	std::string actorName;
};

class MapAction : public Action {
  public:
	std::unique_ptr<Room> prevRoom;
	MapAction(MapActionData a) {
		data = a;
		prevRoom.reset(std::move(a.room));
	}
	MapActionData data;
};

#endif
