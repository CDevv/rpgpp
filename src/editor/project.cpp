#include "project.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "projectScreen.hpp"
#include "room.hpp"
#include "tileset.hpp"
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <string>
#include <vector>

using json = nlohmann::json;

Project::Project(const std::string &path) {
	this->projectPath = GetDirectoryPath(path.c_str());

	char *jsonContent = LoadFileText(path.c_str());
	json j = json::parse(jsonContent);

	this->projectTitle = j.at("title");

	ChangeDirectory(projectPath.c_str());
}

void Project::create(const std::string &dirPath, const std::string &title) {
	json j;
	j["title"] = title;
	std::string fileContent = j.dump(4);

	std::filesystem::path filePath = dirPath;
	filePath /= "proj.rpgpp";
	SaveFileText(filePath.c_str(), fileContent.c_str());

	MakeDirectory(std::filesystem::path(dirPath).append("tilesets").c_str());
	MakeDirectory(std::filesystem::path(dirPath).append("maps").c_str());

	Editor::instance->setProject(filePath);
	Editor::instance->getGui().setScreen(
		std::make_unique<screens::ProjectScreen>(), false);
}

json Project::toJson() {
	json j;
	j["title"] = projectTitle;

	return j;
}

std::string &Project::getTitle() { return projectTitle; }

std::string &Project::getBasePath() { return projectPath; }

std::vector<std::string> Project::getPaths(EngineFileType fileType) {
	std::filesystem::path subdir = projectPath;
	subdir /=
		TextToLower(Editor::instance->getFs().getTypeName(fileType).c_str());
	auto pathList =
		LoadDirectoryFiles(reinterpret_cast<const char *>(subdir.c_str()));
	std::vector<std::string> vec = {};

	for (int i = 0; i < pathList.count; i++) {
		vec.emplace_back(pathList.paths[i]);
	}

	UnloadDirectoryFiles(pathList);

	return vec;
}

GameData Project::generateStruct() {
	GameData data;
	data.title = projectTitle;

	for (auto tileSetPath : getPaths(EngineFileType::FILE_TILESET)) {
		TileSet tileSet(tileSetPath);
		Texture texture = tileSet.getTexture();
		Image image = LoadImageFromTexture(texture);

		int fileSize = 0;
		std::string fileType =
			GetFileExtension(tileSet.getTextureSource().c_str());
		unsigned char *imageData =
			ExportImageToMemory(image, fileType.c_str(), &fileSize);

		TileSetBin tileSetBin;
		tileSetBin.name = GetFileName(tileSetPath.c_str());
		tileSetBin.extension =
			GetFileExtension(tileSet.getTextureSource().c_str());
		for (int i = 0; i < fileSize; i++) {
			tileSetBin.image.push_back(*imageData);
			imageData++;
		}
		tileSetBin.tileSize =
			IVector{static_cast<int>(tileSet.getTileSize().x),
					static_cast<int>(tileSet.getTileSize().y)};
		tileSetBin.dataSize = fileSize;

		data.tilesets[GetFileName(tileSetPath.c_str())] = tileSetBin;

		UnloadImage(image);
	}

	for (auto roomPath : getPaths(EngineFileType::FILE_MAP)) {
		std::unique_ptr<TileMap> map = std::make_unique<TileMap>(roomPath);

		RoomBin roomBin;
		roomBin.name = GetFileName(roomPath.c_str());
		roomBin.tileSetName = GetFileName(map->getTileSetSource().c_str());
		Vector2 worldSize = map->getMaxWorldSize();
		roomBin.width = static_cast<int>(worldSize.x);
		roomBin.height = static_cast<int>(worldSize.y);

		for (int x = 0; x < roomBin.width; x++) {
			std::vector<TileBin> row;
			for (int y = 0; y < roomBin.height; y++) {
				TileBin tile;
				row.push_back(tile);
			}
			roomBin.tiles.push_back(row);
		}
		for (int x = 0; x < roomBin.width; x++) {
			for (int y = 0; y < roomBin.height; y++) {
				Tile tile = map->getTile(x, y);
				Vector2 atlasPos = tile.getAtlasTile().getAtlasCoords();
				Vector2 worldPos = tile.getWorldCoords();

				IVector intAtlas = IVector{static_cast<int>(atlasPos.x),
										   static_cast<int>(atlasPos.y)};
				IVector intWorld = IVector{static_cast<int>(worldPos.x),
										   static_cast<int>(worldPos.y)};

				TileBin tileBin;
				tileBin.atlasPos = intAtlas;
				tileBin.worldPos = intWorld;
				roomBin.tiles[x][y] = tileBin;
			}
		}

		map.reset();

		std::unique_ptr<Room> room = std::make_unique<Room>(roomPath);
		roomBin.startPoint = IVector{static_cast<int>(room->getStartTile().x),
									 static_cast<int>(room->getStartTile().y)};
		for (auto collisionVec : room->getCollisionTiles()) {
			IVector intVec;
			intVec.x = static_cast<int>(collisionVec.x);
			intVec.y = static_cast<int>(collisionVec.y);
			roomBin.collisions.push_back(intVec);
		}
		for (auto interactable : room->getInteractables().getList()) {
			InteractableBin intBin;
			intBin.x = static_cast<int>(interactable->pos.x);
			intBin.y = static_cast<int>(interactable->pos.y);
			intBin.type = static_cast<int>(interactable->type);
			intBin.onTouch = interactable->onTouch;

			switch (interactable->type) {
			case INT_TWO:
				intBin.dialogue =
					(static_cast<IntBase<DiagInt> *>(interactable))
						->get()
						.dialogueSource;
				break;
			case INT_WARPER:
				intBin.dialogue =
					(static_cast<IntBase<WarperInt> *>(interactable))
						->get()
						.targetRoom;
				break;
			default:
				break;
			}
			roomBin.interactables.push_back(intBin);
		}
		for (auto &&prop : room->getProps()) {
			PropInRoomBin pBin;
			pBin.name = prop.getSourcePath();
			pBin.tilePos = IVector{static_cast<int>(prop.getWorldTilePos().x),
								   static_cast<int>(prop.getWorldTilePos().y)};

			if (prop.getHasInteractable()) {
				switch (prop.getInteractable()->type) {
				case INT_TWO:
					pBin.dialogue = ((static_cast<IntBase<DiagInt> *>(
										  prop.getInteractable()))
										 ->get()
										 .dialogueSource);
					break;
				case INT_WARPER:
					pBin.dialogue = ((static_cast<IntBase<WarperInt> *>(
										  prop.getInteractable()))
										 ->get()
										 .targetRoom);
					break;
				default:
					break;
				}
			}
			roomBin.props.push_back(pBin);
		}
		for (auto &&actor : room->getActors()) {
			ActorInRoomBin aBin;
			aBin.name = actor.getSourcePath();
			aBin.tilePos = IVector{static_cast<int>(actor.getTilePosition().x),
								   static_cast<int>(actor.getTilePosition().y)};
			roomBin.actors.push_back(aBin);
		}
		roomBin.musicSource = room->getMusicSource();

		room.reset();
		data.rooms.push_back(roomBin);
	}

	return data;
}

void Project::runProject() {
	std::string editorBasePath = Editor::instance->getFs().getEditorBaseDir();

	std::filesystem::path libPath = editorBasePath;
	libPath /= "game-src";
	libPath /= "lib";
	std::filesystem::path libDest = projectPath;
	std::filesystem::path scriptPath = editorBasePath;
	scriptPath /= "game-src/script.lua";

	std::filesystem::path intepreterPath = editorBasePath;
	intepreterPath /= "execs";
#ifdef __linux
	libPath /= "librpgpplua.so";
	intepreterPath /= "luajit";
	libDest /= "rpgpplua.so";

	std::filesystem::copy_file(
		libPath, libDest, std::filesystem::copy_options::overwrite_existing);

	ChangeDirectory(projectPath.c_str());

	std::string outData;
	char buffer[256];
	FILE *stream;
	stream = popen(TextFormat("%s -l rpgpplua %s", intepreterPath.c_str(),
							  scriptPath.c_str()),
				   "r");
	if (stream) {
		while (!feof(stream)) {
			if (fgets(buffer, 256, stream) != NULL) {
				outData.append(buffer);
			}
		}
		pclose(stream);
	}

	printf("Stream data: \n");
	printf("%s", outData.c_str());
#endif
#ifdef __WIN64
	intepreterPath /= "luajit.exe";
#endif

	// ChangeDirectory(editorBasePath.c_str());
}