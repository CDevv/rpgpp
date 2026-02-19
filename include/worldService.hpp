#ifndef _RPGPP_WORLDSERVICE_H
#define _RPGPP_WORLDSERVICE_H

#include "gamedata.hpp"
#include "player.hpp"
#include "room.hpp"
#include <map>
#include <memory>
#include <string>

/** The WorldService is responsible for containing and drawing the Room. */
class WorldService {
  private:
	/** The current room in this World. */
	std::unique_ptr<Room> room;
	bool lock;
	/** Id for the next room. */
	std::string deferredRoomId;
	bool deferRoomChange;
	/** Frame counter for transition tween */
	int frameCounter;
	bool transitionActive;
	Color transitionColor;
	float alpha;
	bool transitionSecondStage;

  public:
	std::map<std::string, std::string> interactableNames{};
	/** Empty constructor. */
	WorldService();
	/** Get a reference to the current room. */
	Room &getRoom() const;
	/** Set the current room using path to a .rmap file. */
	void setRoom(const std::string_view &filePath);
	/** Set the current room using a RoomBin binary structure. */
	void setRoomBin(RoomBin bin);
	/** Activate transition effect (warper) */
	void doFadeTransition();
	/** Get a reference to the Player object. */
	Player &getPlayer() const;
	/** Update routine. */
	void update();
	/** Draw routine. */
	void draw() const;
	/** Unload routine. */
	void unload() const;
};

#endif
