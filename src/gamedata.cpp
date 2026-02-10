#include "gamedata.hpp"
#include "interactable.hpp"
#include <fstream>

#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <cereal/archives/portable_binary.hpp>

template <class Archive> void serialize(Archive &a, IRect &b) {
	a(b.x, b.y, b.width, b.height);
}

template <class Archive> void serialize(Archive &a, IVector &b) { a(b.x, b.y); }

template <class Archive> void serialize(Archive &a, ActorBin &b) {
	a(b.name, b.tileSetName, b.collision, b.animations);
}

template <class Archive> void serialize(Archive &a, ActorInRoomBin &b) {
	a(b.name, b.tilePos);
}

template <class Archive> void serialize(Archive &a, TileBin &b) {
	a(b.atlasPos, b.worldPos);
}

template <class Archive> void serialize(Archive &a, InteractableBin &b) {
	a(b.typeName, b.scriptPath, b.props);
}

template <class Archive> void serialize(Archive &a, InteractableInRoomBin &b) {
	a(b.x, b.y, b.onTouch, b.propsCbor, b.type);
}

template <class Archive> void serialize(Archive &a, PropInRoomBin &b) {
	a(b.name, b.tilePos, b.onTouch, b.propsCbor);
}

template <class Archive> void serialize(Archive &a, TileSetBin &b) {
	a(b.name, b.extension, b.tileSize, b.image, b.dataSize);
}

template <class Archive> void serialize(Archive &a, ImageBin &b) {
	a(b.data, b.dataSize);
}

template <class Archive> void serialize(Archive &a, MusicBin &b) {
	a(b.relativePath, b.isSound);
}

template <class Archive> void serialize(Archive &a, PropBin &b) {
	a(b.name, b.atlasRect, b.collisionRect, b.imagePath, b.hasInteractable,
	  b.intType);
}

template <class Archive> void serialize(Archive &a, RoomBin &b) {
	a(b.name, b.tileSetName, b.width, b.height, b.startPoint, b.collisions,
	  b.interactables, b.props, b.actors, b.tiles, b.musicSource);
}

template <class Archive> void serialize(Archive &a, DialogueTextSection &b) {
	a(b.key, b.text);
}

template <class Archive> void serialize(Archive &a, DialogueLine &b) {
	a(b.text, b.sections, b.imageId, b.hasPortrait, b.characterName);
}

template <class Archive> void serialize(Archive &a, Dialogue &b) {
	a(b.title, b.lines);
}

template <class Archive> void serialize(Archive &a, GameData &b) {
	a(b.title, b.images, b.tilesets, b.rooms, b.actors, b.props, b.dialogues,
	  b.music, b.interactables, b.scripts);
}

template <class Archive> void serialize(Archive &a, ScriptBin &b) {
	a(b.bytecode);
}

void serializeDataToFile(std::string fileName, GameData data) {
	{
		std::fstream os{fileName, std::ifstream::binary | std::fstream::trunc |
									  std::fstream::out};
		if (!os.is_open()) {
			printf("Cannot open %s for writing. \n", fileName.c_str());
			return;
		}

		cereal::PortableBinaryOutputArchive outa(os);
		outa(data);
	}
	printf("tried to serialize.. \n");
}

GameData deserializeFile(std::string fileName) {
	GameData data{};
	{
		std::ifstream is;
		is.open(fileName, std::ios::in | std::ios::binary);
		cereal::PortableBinaryInputArchive ina(is);
		ina(data);
	}

	return data;
}
