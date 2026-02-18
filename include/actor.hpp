#ifndef _RPGPP_ACTOR_H
#define _RPGPP_ACTOR_H

#include "atlasTile.hpp"
#include "gamedata.hpp"
#include "tileset.hpp"
#include <array>
#include <memory>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <vector>

using json = nlohmann::json;

/** Direction enum, representing an animation state. */
enum Direction {
	RPGPP_DOWN_IDLE = 0,  ///< Down Idle state.
	RPGPP_DOWN = 1,		  ///< Down state.
	RPGPP_UP_IDLE = 2,	  ///< Up Idle state.
	RPGPP_UP = 3,		  ///< Up state.
	RPGPP_LEFT_IDLE = 4,  ///< Left Idle state.
	RPGPP_LEFT = 5,		  ///< Left state.
	RPGPP_RIGHT_IDLE = 6, ///< Right Idle state.
	RPGPP_RIGHT = 7		  ///< Right state.
};

/** The Actor class represents an Actor in the game's world.
 * @see [Direction](Direction.md)
 */
class Actor {
  private:
	std::string sourcePath;
	/** The used TileSet for this Actor's sprites. */
	std::unique_ptr<TileSet> tileSet;
	/** The path to the TileSet file. */
	std::string tileSetSource;
	/** The currently shown tile from the TileSet. */
	AtlasTile tile;
	/** Current position in the world of this Actor. */
	Vector2 position{};
	/** Current tile position of this Actor if such is set. */
	Vector2 tilePosition{};
	int frameCounter{};
	int frameSpeed{};
	int currentFrame{};
	/** Rectangle, representing the collision of this Actor. */
	Rectangle collisionRect{};
	/** An array of the animations. Each std::vector<Vector2> represents the
	 * frames of an animation. */
	std::array<std::unique_ptr<std::vector<Vector2>>, 8> animations;
	/** A Direction enum, showing the current animation that is being played. */
	Direction currentAnimation;

  public:
	/** Empty constructor. */
	Actor() = default;
	/** Constructor that takes a path to the .ractor file. */
	Actor(const std::string &fileName);
	/** Constructor that takes a TileSet, the atlas position of the tile to use,
	 * and the path to the TileSet. */
	Actor(std::unique_ptr<TileSet> tileSet, Vector2 atlasPos,
		  std::string tileSetSource);
	/** Constructor that takes an ActorBin binary structure */
	Actor(ActorBin bin);
	/** Dump this Actor's data to a nlohmann::json object. */
	json dumpJson();
	/** Unload routine. The UnloadTexture function will called here. */
	void unload() const;
	/** Update routine. */
	void update();
	/** Draw routine. */
	void draw() const;
	/** Get the source file's path for this Actor. */
	std::string getSourcePath() const;
	/** Get this actor's position in the world. */
	Vector2 getPosition() const;
	/** Set this Actor's position. */
	void setPosition(Vector2 newPosition);
	/** Set Actor position relative to a tile in the world. */
	void setTilePosition(Vector2 newPosition, Vector2 tileSize);
	/** Get this Actor's tile position. */
	Vector2 getTilePosition() const;
	/** Move the Actor using a velocity vector. */
	void moveByVelocity(Vector2 velocity);
	/** Get the bounding box of this Actor. */
	Rectangle getRect() const;
	/** Get a reference to this Actor's TileSet. */
	TileSet &getTileSet() const;
	/** Set this Actor's TileSet using a path to the tileset file. */
	void setTileSet(const std::string &newTileSetSource);
	/** Get the collision rectangle of this Actor if it was moved by the
	 * velocity vector */
	Rectangle getCollisionRect(Vector2 velocity) const;
	/** Get collision center point. */
	Vector2 getCollisionCenter() const;
	/** Add a frame in the chosen animation. The frame represents an atlas tile
	 * from this Actor's TileSet. */
	void addAnimationFrame(Direction id, Vector2 atlasPos) const;
	/** Remove a frame in the chosen animation, specified by an index. */
	void removeAnimationFrame(Direction id, int frameIndex);
	/** Change a frame in the chosen animation to another atlas tile from the
	 * TileSet. */
	void setAnimationFrame(Direction id, int frameIndex,
						   Vector2 atlasTile) const;
	/** Add multiple frames to the chosen animation. */
	void addAnimationFrames(Direction id,
							const std::vector<std::vector<int>> &frames) const;
	/** Change the Actor's current animation and play it. */
	void changeAnimation(Direction id);
	/** Get the path of the used TileSet. */
	std::string getTileSetSource() const;
	/** Get an array of this Actor's animations */
	std::array<std::vector<Vector2>, 8> getAnimationsRaw() const;
	/** Get a specific animation */
	std::vector<Vector2> getAnimationRaw(Direction id) const;
	/** Get the collision Rectangle of this Actor */
	Rectangle getCollisionRect() const;
	/** Set the Actor's collision Rectangle */
	void setCollisionRect(Rectangle rect);
};

#endif
