#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

class Player;
class TileMap;

#include "game.hpp"
#include "gamedata.hpp"
#include <memory>
#include <vector>
#include <raylib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "actor.hpp"
#include "interactablesContainer.hpp"
#include "collisionsContainer.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "interactable.hpp"

/**
 * This class represents a game's room. It can contain the Player, interactables, collisions and the TileMap
 */
class Room {
private:
    int worldTileSize;
    /** Container of the interactable tiles */
    std::unique_ptr<InteractablesContainer> interactables;
    /** Container of the collision tiles */
    std::unique_ptr<CollisionsContainer> collisions;
    /** This Room's TileMap, which contains all placed tiles. */
    std::unique_ptr<TileMap> tileMap;
    /** A collection of all Actors in this Room */
    std::unique_ptr<std::vector<Actor>> actors;
    /** This Room's only Player. */
    std::unique_ptr<Player> player;
public:
    /** Empty constructor */
    Room();
    /** Construct a Room from an .rmap file */
    Room(std::string fileName);
    /** Construct a Room by using a TileMap pointer */
    Room(std::unique_ptr<TileMap> tileMap);
    /** Construct a Room by using the RoomBin binary structure
     *@param bin The binary structure to use */
    Room(RoomBin bin);
    /** Dump the room's data into a nlohmann::json object for later use
     * Useful for dumping into a .json file.
     * @returns The JSON object */
    json dumpJson();
    /** Unload used resources. Typically unloads used Textures by calling raylib's UnloadTexture */
    void unload();
    /** Update routine for this Room. */
    void update();
    /** Draw routine for this Room. */
    void draw();
    /** Add an actor to this Room
     * @param actor The actor to be added to the Room's collection.
     */
    void addActor(Actor actor);
    /** Add the Player into this Room. A unique_ptr is passed whose ownership will be moved into the Room.
     * @param player A pointer to the player that will be moved into this Room.
     */
    void addPlayer(std::unique_ptr<Player> player);
    /** Get a reference to the Player object.
     * @returns The Player in this Room.
     */
    Player& getPlayer();
    /** Get a pointer to this Room's TileMap.
     * @returns A pointer to the room's TileMap.
     */
    TileMap *getTileMap();

    std::vector<Vector2> getCollisionTiles();
    std::vector<Interactable> getInteractableTiles();
    /** Get a reference to the CollisionsContainer of this Room. */
    CollisionsContainer& getCollisions();
    /** Get a reference to the InteractablesContainer of this Room. */
    InteractablesContainer& getInteractables();
};

#endif
