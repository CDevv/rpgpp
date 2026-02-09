#ifndef _RPGPP_MAPACTION_H
#define _RPGPP_MAPACTION_H

#include "action.hpp"
#include "room.hpp"
#include "roomToolbox.hpp"
#include "roomView.hpp"
#include "tilemap.hpp"
#include <memory>
#include <utility>

struct MapActionData {
	RoomView *view;
	Room *room;
	RoomLayer layer;
	InteractableType interactable;
	Vector2 worldTile;
	Vector2 tile;
	Vector2 prevTile;
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