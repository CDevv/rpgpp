#ifndef _RPGPP_GAMEDATA_H
#define _RPGPP_GAMEDATA_H

#include <raylib.h>
#include <vector>
#include <array>
#include <map>
#include <cstdint>
#include <string>

#include "dialogueBalloon.hpp"

#define RPGPP_DRAW_MULTIPLIER (3)

struct IVector
{
	int x;
	int y;
};

struct IRect
{
	int x;
	int y;
	int width;
	int height;
};

struct ActorBin
{
	std::string name;
	std::string tileSetName;
	IRect collision;
	std::array<std::vector<IVector>, 8> animations;
};

struct TileBin
{
	IVector atlasPos;
	IVector worldPos;
};

struct InteractableBin
{
	int x;
	int y;
	int type;
	bool onTouch;
	Dialogue dialogue;
};

struct TileSetBin
{
	std::string name;
	std::string extension;
	IVector tileSize;
	std::vector<unsigned char> image;
	int dataSize;
};

struct RoomBin
{
	std::string name;
	std::string tileSetName;
	int width;
	int height;
	std::vector<IVector> collisions;
	std::vector<InteractableBin> interactables;
	std::vector<std::vector<TileBin>> tiles;
};

struct GameData
{
	std::string title;
	std::map<std::string, TileSetBin> tilesets;
	std::vector<RoomBin> rooms;
	std::vector<ActorBin> actors;
};

struct DataSerialization
{
	std::vector<uint8_t> bytes;
	int count;
};

DataSerialization serializeGameData(GameData data);
void serializeDataToFile(std::string fileName, GameData data);
GameData deserializeFile(std::string fileName);

#endif
