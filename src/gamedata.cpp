#include "gamedata.hpp"
#include <alpaca/alpaca.h>
#include <filesystem>

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
	std::ofstream os;
    os.open(fileName, std::ios::out | std::ios::binary);
    auto bytes_written = alpaca::serialize(data, os);
    os.close();
}

GameData deserializeFile(std::string fileName)
{
	auto size = std::filesystem::file_size(fileName);
    std::error_code error;
    std::ifstream is;
    is.open(fileName, std::ios::in | std::ios::binary);
    auto data = alpaca::deserialize<GameData>(is, size, error);
    is.close();

    return data;
}
