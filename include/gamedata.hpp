#ifndef _RPGPP_GAMEDATA_H
#define _RPGPP_GAMEDATA_H

#include <array>
#include <cstdint>
#include <map>
#include <raylib.h>
#include <string>
#include <vector>

#include "dialogueBalloon.hpp"

#define RPGPP_DRAW_MULTIPLIER (3)

struct IVector {
	int x;
	int y;
};

struct IRect {
	int x;
	int y;
	int width;
	int height;
};

struct ActorBin {
	std::string name;
	std::string tileSetName;
	IRect collision;
	std::array<std::vector<IVector>, 8> animations;
};

struct ActorInRoomBin {
	std::string name;
	IVector tilePos;
};

struct TileBin {
	IVector atlasPos;
	IVector worldPos;
};

struct InteractableBin {
	std::string typeName;
	std::string scriptPath;
	std::vector<std::uint8_t> props;
};

struct ScriptBin {
	std::string bytecode;
};

struct InteractableInRoomBin {
	int x;
	int y;
	bool onTouch;
	std::string type;
	std::vector<std::uint8_t> propsCbor;
};

struct PropInRoomBin {
	std::string name;
	IVector tilePos;
	bool onTouch;
	std::vector<std::uint8_t> propsCbor;
};

struct TileSetBin {
	std::string name;
	std::string extension;
	IVector tileSize;
	std::vector<unsigned char> image;
	int dataSize;
};

struct ImageBin {
	std::vector<unsigned char> data;
	int dataSize;
};

struct MusicBin {
	std::string relativePath;
	bool isSound;
};

struct PropBin {
	std::string name;
	IRect atlasRect;
	IRect collisionRect;
	std::string imagePath;
	bool hasInteractable;
	std::string intType;
};

struct RoomBin {
	std::string name;
	std::string tileSetName;
	int width;
	int height;
	IVector startPoint;
	std::vector<IVector> collisions;
	std::vector<InteractableInRoomBin> interactables;
	std::vector<PropInRoomBin> props;
	std::vector<ActorInRoomBin> actors;
	std::vector<std::vector<TileBin>> tiles;
	std::string musicSource;
};

struct GameBinSettings {
	std::string playerActor;
};

struct GameData {
	std::string title;
	std::map<std::string, ImageBin> images;
	std::map<std::string, TileSetBin> tilesets;
	std::map<std::string, InteractableBin> interactables;
	std::vector<RoomBin> rooms;
	std::vector<ActorBin> actors;
	std::vector<PropBin> props;
	std::map<std::string, Dialogue> dialogues;
	std::map<std::string, MusicBin> music;
	std::map<std::string, ScriptBin> scripts;
};

struct DataSerialization {
	std::vector<uint8_t> bytes;
	int count;
};

void serializeDataToFile(std::string fileName, GameData data);
GameData deserializeFile(std::string fileName);

#endif
