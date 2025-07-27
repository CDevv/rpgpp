#ifndef _RPGPP_GAMEDATA_H
#define _RPGPP_GAMEDATA_H

#include <raylib.h>
#include <vector>
#include <map>
#include <cstdint>
#include <string>

struct IVector 
{
	int x;
	int y;
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
};

struct TileSetBin
{
	std::string name;
	std::string extension;
	int tileSize;
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