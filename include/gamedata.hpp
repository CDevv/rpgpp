#ifndef _RPGPP_GAMEDATA_H
#define _RPGPP_GAMEDATA_H

#include <raylib.h>
#include <vector>
#include <cstdint>
#include <string>

struct TileSetBin
{
	std::string name;
	int tileSize;
	std::vector<unsigned char> image;
	int dataSize;
};

struct GameData
{
	std::string title;
	std::vector<TileSetBin> tilesets;
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