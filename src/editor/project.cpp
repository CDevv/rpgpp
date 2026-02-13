#include "project.hpp"
#include "dialogueParser.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "room.hpp"
#include "screens/projectScreen.hpp"
#include "services/fileSystemService.hpp"
#include "tileset.hpp"
#include <cstdio>
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
	SaveFileText(filePath.u8string().c_str(), fileContent.c_str());

	MakeDirectory(std::filesystem::path(dirPath).append("tilesets").u8string().c_str());
	MakeDirectory(std::filesystem::path(dirPath).append("maps").u8string().c_str());

	Editor::instance->setProject(filePath.u8string());
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
			InteractableInRoomBin intBin;
			intBin.x = static_cast<int>(interactable->getWorldPos().x);
			intBin.y = static_cast<int>(interactable->getWorldPos().y);
			intBin.type = interactable->getType();
			intBin.onTouch = interactable->isOnTouch();

			intBin.propsCbor =
				nlohmann::json::to_cbor(interactable->getProps());

			roomBin.interactables.push_back(intBin);
		}
		for (auto &&prop : room->getProps()) {
			PropInRoomBin pBin;
			pBin.name = prop.getSourcePath();
			pBin.tilePos = IVector{static_cast<int>(prop.getWorldTilePos().x),
								   static_cast<int>(prop.getWorldTilePos().y)};

			pBin.propsCbor =
				nlohmann::json::to_cbor(prop.getInteractable()->getProps());
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

	for (auto actorPath : getPaths(EngineFileType::FILE_ACTOR)) {
		std::unique_ptr<Actor> actor = std::make_unique<Actor>(actorPath);

		ActorBin actorBin;
		actorBin.name = GetFileName(actorPath.c_str());
		actorBin.tileSetName = GetFileName(actor->getTileSetSource().c_str());

		Rectangle collisionRect = actor->getCollisionRect();
		actorBin.collision = IRect{static_cast<int>(collisionRect.x),
								   static_cast<int>(collisionRect.y),
								   static_cast<int>(collisionRect.width),
								   static_cast<int>(collisionRect.height)};
		std::array<std::vector<Vector2>, 8> animations =
			actor->getAnimationsRaw();
		for (int i = 0; i < 8; i++) {
			for (int frameIndex = 0; frameIndex < animations[i].size();
				 frameIndex++) {
				Vector2 vec = animations[i][frameIndex];
				IVector intVec =
					IVector{static_cast<int>(vec.x), static_cast<int>(vec.y)};

				actorBin.animations[i].push_back(intVec);
			}
		}

		data.actors.push_back(actorBin);
	}

	for (auto diagPath : getPaths(EngineFileType::FILE_DIALOGUE)) {
		std::string diagText = LoadFileText(diagPath.c_str());
		Dialogue diag = parseDialogueText(diagText);

		diag.title = GetFileNameWithoutExt(diagPath.c_str());
		data.dialogues[GetFileNameWithoutExt(diagPath.c_str())] = diag;
	}

	for (auto imagePath : getPaths(EngineFileType::FILE_IMAGE)) {
		Image img = LoadImage(imagePath.c_str());
		ImageBin bin;

		int fileSize = 0;

		unsigned char *imgData = ExportImageToMemory(
			img, GetFileExtension(imagePath.c_str()), &fileSize);
		for (int i = 0; i < fileSize; i++) {
			bin.data.push_back(*imgData);
			imgData++;
		}
		bin.dataSize = fileSize;

		UnloadImage(img);

		data.images[GetFileName(imagePath.c_str())] = bin;
	}

	for (auto fontPath : getPaths(EngineFileType::FILE_FONT)) {
	}

	for (auto soundPath : getPaths(EngineFileType::FILE_SOUND)) {
	}

	for (auto musicPath : getPaths(EngineFileType::FILE_MUSIC)) {
	}

	for (auto propPath : getPaths(EngineFileType::FILE_PROP)) {
		Prop prop = Prop(propPath);

		PropBin bin;
		bin.name = propPath;
		bin.atlasRect = IRect{static_cast<int>(prop.getAtlasRect().x),
							  static_cast<int>(prop.getAtlasRect().y),
							  static_cast<int>(prop.getAtlasRect().width),
							  static_cast<int>(prop.getAtlasRect().height)};
		bin.collisionRect =
			IRect{static_cast<int>(prop.getCollisionRect().x),
				  static_cast<int>(prop.getCollisionRect().y),
				  static_cast<int>(prop.getCollisionRect().width),
				  static_cast<int>(prop.getCollisionRect().height)};
		bin.imagePath = std::string(prop.getImagePath());
		bin.hasInteractable = prop.getHasInteractable();
		bin.intType = prop.getInteractable()->getType();

		data.props.push_back(bin);
	}

	// built in insteractables
	std::filesystem::path interactablesDir =
		Editor::instance->getFs().getEditorBaseDir();
	interactablesDir /= "resources";
	interactablesDir /= "interactables";

	auto list = LoadDirectoryFiles(interactablesDir.u8string().c_str());
	for (int i = 0; i < list.count; i++) {
		std::string intPath = list.paths[i];
		Interactable inter(intPath);

		InteractableBin bin;
		bin.typeName = inter.getType();
		bin.scriptPath = inter.getScriptSourcePath();
		bin.props = nlohmann::json::to_cbor(inter.getProps());

		printf("%s \n", bin.typeName.c_str());

		data.interactables[inter.getType()] = bin;
	}

	// scripts
	std::filesystem::path scriptsDir =
		Editor::instance->getFs().getEditorBaseDir();
	scriptsDir /= "resources";
	scriptsDir /= "scripts";
	auto scriptsList = LoadDirectoryFiles(scriptsDir.u8string().c_str());
	for (int i = 0; i < scriptsList.count; i++) {
		std::string scriptPath = scriptsList.paths[i];
		auto scriptText = LoadFileText(scriptPath.c_str());

		/*
		sol::state lua;
		auto lr = lua.load(scriptText);

		SOL_ASSERT(lr.valid());

		sol::protected_function target = lr.get<sol::protected_function>();
		auto bytecode = target.dump();
		auto sv = bytecode.as_string_view();
		*/

		ScriptBin bin;
		// bin.bytecode = std::string(sv.data());
		bin.bytecode = scriptText;

		data.scripts[GetFileName(scriptPath.c_str())] = bin;
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
	scriptPath /= "resources/scripts/script.lua";

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
	/*
if (stream) {
while (!feof(stream)) {
if (fgets(buffer, 256, stream) != NULL) {
 outData.append(buffer);
}
}
pclose(stream);
}
*/

	printf("Stream data: \n");
	printf("%s", outData.c_str());
#endif
#ifdef __WIN64
	intepreterPath /= "luajit.exe";
#endif

	// ChangeDirectory(editorBasePath.c_str());
}
