#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

constexpr const char *DEFAULT_PLAYER_PATH = "actors/playerActor.ractor";

class Player;
class TileMap;

#include "gamedata.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <string>
#include <vector>

using json = nlohmann::json;
#include "actor.hpp"
#include "collisionsContainer.hpp"
#include "interactablesContainer.hpp"
#include "player.hpp"
#include "prop.hpp"
#include "tilemap.hpp"

/**
 * This class represents a game's room. It can contain the Player,
 * interactables, collisions and the TileMap
 */
class Room : public ISaveable {
  private:
	bool lock;
	int worldTileSize;
	/** The camera. */
	Camera2D camera{};
	/** Start point of the player. (tile position) */
	Vector2 startTile{};
	/** Source of the background music for this room. */
	std::string musicSource;
	/** Container of the interactable tiles */
	std::unique_ptr<InteractablesContainer> interactables;
	/** Container of the collision tiles */
	std::unique_ptr<CollisionsContainer> collisions;
	/** A collection of the Props in this Room. */
	std::unique_ptr<std::vector<Prop>> props;
	/** This Room's TileMap, which contains all placed tiles. */
	std::unique_ptr<TileMap> tileMap;
	/** A collection of all Actors in this Room */
	std::unique_ptr<std::vector<Actor>> actors;
	/** This Room's only Player. */
	std::unique_ptr<Player> player;
	void updateCamera();

  public:
	/** Empty constructor */
	Room();
	/** Construct a Room from an .rmap file */
	Room(const std::string &fileName, int tileSize = 48);
	/** Construct a Room by using a TileMap pointer */
	Room(std::unique_ptr<TileMap> tileMap);
	/** Construct a Room by using the RoomBin binary structure
	 *@param bin The binary structure to use */
	Room(const RoomBin &bin);
	/** Dump the room's data into a nlohmann::json object for later use
	 * Useful for dumping into a .json file.
	 * @returns The JSON object */
	json dumpJson();
	/** Unload used resources. Typically unloads used Textures by calling
	 * raylib's UnloadTexture */
	void unload() const;
	/** Update routine for this Room. */
	void update();
	/** Draw routine for this Room. */
	void draw() const;
	void setWorldTileSize(int tileSize);
	int getWorldTileSize() const;
	void setLock(bool val);
	/** Add the Player into this Room. A unique_ptr is passed whose ownership
	 * will be moved into the Room.
	 * @param newPlayer A pointer to the player that will be moved into this
	 * Room.
	 */
	void addPlayer(std::unique_ptr<Player> newPlayer);
	/** Get a reference to the Player object.
	 * @returns The Player in this Room.
	 */
	Player &getPlayer() const;
	/** Get a pointer to this Room's TileMap.
	 * @returns A pointer to the room's TileMap.
	 */
	TileMap *getTileMap() const;
	void setTileMap(TileMap *newTileMap);

	/** Get filename of the source music. */
	std::string getMusicSource() const;
	/** Set music source. */
	void setMusicSource(const std::string_view &newMusicSource);
	/** Get the start tile position. */
	Vector2 getStartTile() const;
	/** Set the start tile position. */
	void setStartTile(Vector2 newStartTile);

	std::vector<Vector2> getCollisionTiles() const;
	/** Get a reference to the CollisionsContainer of this Room. */
	CollisionsContainer &getCollisions() const;
	/** Get a reference to the InteractablesContainer of this Room. */
	InteractablesContainer &getInteractables() const;

	/** Get a reference to the Props container (vector). */
	std::vector<Prop> &getProps() const;
	/** Add a Prop to this Room. */
	void addProp(Prop prop) const;
	/** Remove a prop from this room using a world tile position. */
	void removeProp(Vector2 worldPos) const;
	/** Get the Prop at the specified tile position. */
	Prop *getPropAt(Vector2 worldPos) const;

	/** Get a refernece to the collection of Actors. */
	std::vector<Actor> &getActors() const;
	/** Add an actor to this Room
	 * @param actor The actor to be added to the Room's collection.
	 */
	void addActor(Actor actor) const;
	/** Remove an Actor using a tilePosition. */
	void removeActor(Vector2 tilePosition) const;
};

#endif
