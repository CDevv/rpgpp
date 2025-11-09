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

struct ActorInRoomBin
{
    std::string name;
    IVector tilePos;
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
	std::string dialogue;
};

struct PropInRoomBin
{
    std::string name;
    IVector tilePos;
    bool onTouch;
    std::string dialogue;
};

struct TileSetBin
{
	std::string name;
	std::string extension;
	IVector tileSize;
	std::vector<unsigned char> image;
	int dataSize;
};

struct ImageBin
{
    std::vector<unsigned char> data;
    int dataSize;
};

struct MusicBin
{
    std::string relativePath;
    bool isSound;
};

struct PropBin
{
    std::string name;
    IRect atlasRect;
    IRect collisionRect;
    std::string imagePath;
    bool hasInteractable;
    int intType;
};

struct RoomBin
{
	std::string name;
	std::string tileSetName;
	int width;
	int height;
	IVector startPoint;
	std::vector<IVector> collisions;
	std::vector<InteractableBin> interactables;
	std::vector<PropInRoomBin> props;
	std::vector<ActorInRoomBin> actors;
	std::vector<std::vector<TileBin>> tiles;
	std::string musicSource;
};

struct GameData
{
	std::string title;
	std::map<std::string, ImageBin> images;
	std::map<std::string, TileSetBin> tilesets;
	std::vector<RoomBin> rooms;
	std::vector<ActorBin> actors;
	std::vector<PropBin> props;
	std::map<std::string, Dialogue> dialogues;
	std::map<std::string, MusicBin> music;
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
