#include "gamedata.hpp"
#include <alpaca/alpaca.h>
#include <filesystem>

/*
#include <bitsery/bitsery.h>
#include <bitsery/adapter/stream.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/array.h>
#include <bitsery/traits/string.h>
*/

#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>

#include <cereal/archives/portable_binary.hpp>

/*
using Buffer = std::vector<uint8_t>;
using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

template <typename S>
void serialize(S& s, IVector& i)
{
    s.value4b(i.x);
    s.value4b(i.y);
}

template <typename S>
void serialize(S& s, IRect& i)
{
    s.value4b(i.x);
    s.value4b(i.y);
    s.value4b(i.width);
    s.value4b(i.height);
}

template <typename S>
void serialize(S& s, ActorBin& i)
{
    s.text1b(i.name, 512);
    s.text1b(i.tileSetName, 512);
    s.object(i.collision);
    //s.container(i.animations, 8);
}

template <typename S>
void serialize(S& s, ActorInRoomBin& i)
{
    s.text1b(i.name, 512);
    s.object(i.tilePos);
}

template <typename S>
void serialize(S& s, TileBin& i)
{
    s.object(i.atlasPos);
    s.object(i.worldPos);
}

template <typename S>
void serialize(S& s, InteractableBin& i)
{
    s.value4b(i.x);
    s.value4b(i.y);
    s.value4b(i.type);
    s.value1b(i.onTouch);
    s.text1b(i.dialogue, 1024);
}

template <typename S>
void serialize(S& s, PropInRoomBin& i)
{
    s.text1b(i.name, 512);
    s.object(i.tilePos);
    s.value1b(i.onTouch);
    s.text1b(i.dialogue, 1024);
}

template <typename S>
void serialize(S& s, TileSetBin& i)
{
    s.text1b(i.name, 512);
    s.text1b(i.extension, 10);
    s.object(i.tileSize);
    s.container1b(i.image, 150);
    s.value4b(i.dataSize);
}

template <typename S>
void serialize(S& s, ImageBin& i)
{
    s.container1b(i.data, 16000);
    s.value4b(i.dataSize);
}

template <typename S>
void serialize(S& s, MusicBin& i)
{
    s.text1b(i.relativePath, 1024);
    s.value1b(i.isSound);
}

template <typename S>
void serialize(S& s, PropBin& i)
{
    s.text1b(i.name, 512);
    s.object(i.atlasRect);
    s.object(i.collisionRect);
    s.text1b(i.imagePath, 512);
    s.value1b(i.hasInteractable);
    s.value4b(i.intType);
}

template <typename S>
void serialize(S& s, RoomBin& i)
{
    s.text1b(i.name, 512);
    s.text1b(i.tileSetName, 512);
    s.value4b(i.width);
    s.value4b(i.height);
    s.object(i.startPoint);
    s.container(i.collisions, 300);
    s.container(i.interactables, 300);
    s.container(i.props, 300);
    s.container(i.actors, 300);
    s.container(i.tiles, 400);
    s.text1b(i.musicSource, 512);
}

template <typename S>
void serialize(S& s, GameData& i)
{
    s.text1b(i.title, 512);
    s.container(i.rooms, 200);
    s.container(i.actors, 200);
    s.container(i.props, 200);
}
*/

template<class Archive>
void serialize(Archive& a, IRect& b)
{
    a(b.x, b.y, b.width, b.height);
}

template<class Archive>
void serialize(Archive& a, IVector& b)
{
    a(b.x, b.y);
}

template<class Archive>
void serialize(Archive& a, ActorBin& b)
{
    a(b.name, b.tileSetName, b.collision, b.animations);
}

template<class Archive>
void serialize(Archive& a, ActorInRoomBin& b)
{
    a(b.name, b.tilePos);
}

template<class Archive>
void serialize(Archive& a, TileBin& b)
{
   a(b.atlasPos, b.worldPos);
}

template<class Archive>
void serialize(Archive& a, InteractableBin& b)
{
   a(b.x, b.y, b.onTouch, b.type, b.dialogue);
}

template<class Archive>
void serialize(Archive& a, PropInRoomBin& b)
{
   a(b.name, b.tilePos, b.onTouch, b.dialogue);
}

template<class Archive>
void serialize(Archive& a, TileSetBin& b)
{
   a(b.name, b.extension, b.tileSize, b.image, b.dataSize);
}

template<class Archive>
void serialize(Archive& a, ImageBin& b)
{
   a(b.data, b.dataSize);
}

template<class Archive>
void serialize(Archive& a, MusicBin& b)
{
   a(b.relativePath, b.isSound);
}

template<class Archive>
void serialize(Archive& a, PropBin& b)
{
   a(b.name, b.atlasRect, b.collisionRect, b.imagePath, b.hasInteractable, b.intType);
}

template<class Archive>
void serialize(Archive& a, RoomBin& b)
{
   a(b.name, b.tileSetName, b.width, b.height, b.startPoint, b.collisions, b.interactables, b.props, b.actors, b.tiles, b.musicSource);
}

template<class Archive>
void serialize(Archive& a, DialogueTextSection& b)
{
   a(b.key, b.text);
}

template<class Archive>
void serialize(Archive& a, DialogueLine& b)
{
   a(b.text, b.sections, b.imageId, b.hasPortrait, b.characterName);
}

template<class Archive>
void serialize(Archive& a, Dialogue& b)
{
   a(b.title, b.lines);
}

template<class Archive>
void serialize(Archive& a, GameData& b)
{
   a(b.title, b.images, b.tilesets, b.rooms, b.actors, b.props, b.dialogues, b.music);
}

DataSerialization serializeGameData(GameData data)
{
	std::vector<uint8_t> bytes;
	auto bytesWritten = alpaca::serialize(data, bytes);

	DataSerialization result;
	result.bytes = bytes;
	result.count = bytesWritten;

	return result;
}

void serializeDataToFile(std::string fileName, GameData data)
{
    /*
    IRect testR { 10, 10, 100, 250 };
    IRect res;

    Buffer buffer;

    auto writtenSize = bitsery::quickSerialization<OutputAdapter>(buffer, testR);

    auto state = bitsery::quickDeserialization<InputAdapter>(
    { buffer.begin(), writtenSize }, res);

    assert(state.first == bitsery::ReaderError::NoError && state.second);

    printf("rect.x: %i \n", res.x);

	std::ofstream os;
    os.open(fileName, std::ios::out | std::ios::binary);
    auto bytes_written = alpaca::serialize(data, os);
    os.close();
    */

    /*
    std::fstream s{ fileName, s.binary | s.trunc | s.out };
    if (!s.is_open()) {
        printf("Cannot open %s for writing. \n", fileName.c_str());
        return;
    }

    bitsery::Serializer<bitsery::OutputBufferedStreamAdapter> ser{ s };
    ser.object(data);
    ser.adapter().flush();
    s.close();
    */

    {
        std::fstream os{ fileName, os.binary | os.trunc | os.out };
        if (!os.is_open()) {
            printf("Cannot open %s for writing. \n", fileName.c_str());
            return;
        }

        IRect testR { 10, 10, 100, 250 };
        cereal::PortableBinaryOutputArchive outa(os);
        outa(data);
    }
    printf("tried to serialize.. \n");
}

GameData deserializeFile(std::string fileName)
{
    /*
	auto size = std::filesystem::file_size(fileName);
    std::error_code error;
    std::ifstream is;
    is.open(fileName, std::ios::in | std::ios::binary);
    auto data = alpaca::deserialize<GameData>(is, size, error);
    is.close();

    if (error.value() != 0) {
        printf("deserializeFile error: %s \n", error.message().c_str());
    }

    return data;
    */

    GameData data{};
    {
        std::ifstream is;
        is.open(fileName, std::ios::in | std::ios::binary);
        cereal::PortableBinaryInputArchive ina(is);
        ina(data);
    }

    return data;
}
