#include "project.hpp"
#include "conversion.hpp"
#include "dialogue.hpp"
#include "dialogueParser.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "room.hpp"
#include "screens/projectScreen.hpp"
#include "services/fileSystemService.hpp"
#include "tileset.hpp"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <string>
#include <vector>

#ifdef _WIN64

#include "fix_win32_compatibility.h"
#include <fileapi.h>
#include <handleapi.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <namedpipeapi.h>
#include <processenv.h>
#include <processthreadsapi.h>
#include <winbase.h>
#include <windows.h>
#include <winnt.h>

#else

#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#endif

using json = nlohmann::json;

Project::Project(const std::string &path) {
	this->projectPath = GetDirectoryPath(path.c_str());

	char *jsonContent = LoadFileText(path.c_str());
	json j = json::parse(jsonContent);

	this->projectTitle = j.at("title");

	ChangeDirectory(projectPath.c_str());
	UnloadFileText(jsonContent);
}

void Project::create(const std::string &dirPath, const std::string &title) {
	json j = json::object();
	j["title"] = title;
	std::string fileContent = j.dump();

	std::filesystem::path filePath = dirPath;
	filePath /= "proj.rpgpp";
	SaveFileText(filePath.u8string().c_str(), fileContent.c_str());

	MakeDirectory(
		std::filesystem::path(dirPath).append("tilesets").u8string().c_str());
	MakeDirectory(
		std::filesystem::path(dirPath).append("maps").u8string().c_str());

	Editor::instance->setProject(filePath.u8string());
	Editor::instance->getGui().setScreen(
		std::make_unique<screens::ProjectScreen>(), false);
}

json Project::toJson() {
	json j = json::object();
	j["title"] = projectTitle;

	return j;
}

std::string &Project::getTitle() { return projectTitle; }

std::string &Project::getBasePath() { return projectPath; }

std::vector<std::string> Project::getPaths(EngineFileType fileType) {
	std::filesystem::path subdir = projectPath;
	subdir /=
		TextToLower(Editor::instance->getFs().getTypeName(fileType).c_str());

	assert(subdir.string().empty() == false && "directory path is empty");

	auto pathList = LoadDirectoryFiles(subdir.string().c_str());
	std::vector<std::string> vec = {};

	for (int i = 0; i < pathList.count; i++) {
		vec.emplace_back(pathList.paths[i]);
	}

	UnloadDirectoryFiles(pathList);

	return vec;
}

std::string Project::getResourcePath(EngineFileType fileType,
									 const std::string &fileName) {
	std::filesystem::path subdir = projectPath;
	subdir /=
		TextToLower(Editor::instance->getFs().getTypeName(fileType).c_str());
	subdir /= fileName;

	return subdir.string();
}

std::map<std::string, std::string> Project::getInteractableNames() {
	std::map<std::string, std::string> map{};

	// built-in interactables
	std::filesystem::path interactablesDir =
		Editor::instance->getFs().getEditorBaseDir();
	interactablesDir /= "resources";
	interactablesDir /= "interactables";

	auto list = LoadDirectoryFiles(interactablesDir.u8string().c_str());
	for (int i = 0; i < list.count; i++) {
		std::string intPath = list.paths[i];
		Interactable inter(intPath);

		map[intPath.c_str()] = inter.getDisplayTitle();
	}
	UnloadDirectoryFiles(list);

	return map;
}

std::vector<std::string> Project::getPropsNames() {
	std::vector<std::string> vec;
	for (auto propPath : getPaths(EngineFileType::FILE_PROP)) {
		vec.push_back(propPath);
	}
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
		for (auto [pos, obj] : room->getCollisions().getObjects()) {
			IVector intVec;
			intVec.x = static_cast<int>(pos.x);
			intVec.y = static_cast<int>(pos.y);
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
		for (auto &[pos, prop] : room->getProps().getObjects()) {
			PropInRoomBin pBin;
			pBin.name = prop->getSourcePath();
			pBin.tilePos = fromVector2(prop->getWorldTilePos());

			pBin.propsCbor =
				nlohmann::json::to_cbor(prop->getInteractable()->getProps());
			roomBin.props.push_back(pBin);
		}
		for (auto &[pos, actor] : room->getActors().getObjects()) {
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

		data.actors[GetFileNameWithoutExt(actorPath.c_str())] = actorBin;
	}

	for (auto diagPath : getPaths(EngineFileType::FILE_DIALOGUE)) {
		Dialogue dialogue(diagPath);
		DialogueBin diag = dialogue.getData();

		diag.title = GetFileNameWithoutExt(diagPath.c_str());
		data.dialogues[GetFileNameWithoutExt(diagPath.c_str())] = diag;
	}

	for (auto imagePath : getPaths(EngineFileType::FILE_IMAGE)) {
		Image img = LoadImage(imagePath.c_str());
		ImageBin bin;
		bin.ext = GetFileExtension(imagePath.c_str());

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
		MusicBin soundBin;

		int dataSize = 0;
		auto fileData = LoadFileData(soundPath.c_str(), &dataSize);

		for (int i = 0; i < dataSize; i++) {
			soundBin.fileData.push_back(fileData[i]);
		}

		soundBin.fileExt = GetFileExtension(soundPath.c_str());
		soundBin.isSound = true;
		data.music[GetFileNameWithoutExt(soundPath.c_str())] = soundBin;

		UnloadFileData(fileData);
	}

	for (auto musicPath : getPaths(EngineFileType::FILE_MUSIC)) {
		MusicBin musicBin;

		int dataSize = 0;
		auto fileData = LoadFileData(musicPath.c_str(), &dataSize);

		for (int i = 0; i < dataSize; i++) {
			musicBin.fileData.push_back(fileData[i]);
		}

		musicBin.fileExt = GetFileExtension(musicPath.c_str());
		musicBin.isSound = false;
		data.music[GetFileNameWithoutExt(musicPath.c_str())] = musicBin;

		UnloadFileData(fileData);
	}

	for (auto propPath : getPaths(EngineFileType::FILE_PROP)) {
		Prop prop = Prop(propPath);

		PropBin bin;
		bin.name = GetFileNameWithoutExt(propPath.c_str());
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
	UnloadDirectoryFiles(list);

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
		UnloadFileText(scriptText);
	}
	UnloadDirectoryFiles(scriptsList);

	return data;
}

void Project::runProject() {
	// Generate the bin first
	auto bin = generateStruct();

	std::filesystem::path binPath = projectPath;
	binPath /= "game.bin";
	serializeDataToFile(binPath.u8string(), bin);

	// run project
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

	std::string cmdLine = TextFormat(
		"%s -l rpgpplua %s", intepreterPath.c_str(), scriptPath.c_str());
	printf("%s \n", cmdLine.c_str());

	char buffer[256];
	FILE *stream;
	stream = popen(TextFormat("%s -l rpgpplua %s", intepreterPath.c_str(),
							  scriptPath.c_str()),
				   "r");

#endif
#ifdef _WIN64
	intepreterPath /= "luajit.exe";

	const std::filesystem::path rpgppDllPath = "rpgpplua.dll";
	libPath /= rpgppDllPath;
	libDest /= rpgppDllPath;

	std::filesystem::copy_file(
		libPath, libDest, std::filesystem::copy_options::overwrite_existing);

	ChangeDirectory(projectPath.c_str());

	// note: compared to linux, you have to add .string() to every single one of
	// these paths.
	std::string cmdLine =
		TextFormat("%s -l rpgpplua %s", intepreterPath.string().c_str(),
				   scriptPath.string().c_str());

	HANDLE outFile = nullptr;

	outFile = CreateFile("playtest.log", GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
						 OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	SetStdHandle(STD_OUTPUT_HANDLE, outFile);
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdOutput = outFile;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bool success = CreateProcess(NULL, cmdLine.data(), NULL, NULL, true, 0,
								 NULL, NULL, &siStartInfo, &piProcInfo);

	if (!success) {
		printf("Child process doesn't work. \n");
	} else {
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		CloseHandle(outFile);
	}

#endif
}

void Project::buildProject() {
	// Generate the bin file first
	auto bin = generateStruct();

	std::filesystem::path binPath = projectPath;
	binPath /= "game.bin";
	serializeDataToFile(binPath.u8string(), bin);

	// Copy base game file
	std::filesystem::path baseGamePath =
		Editor::instance->getFs().getEditorBaseDir();

	std::filesystem::path resultPath = projectPath;

#ifdef _WIN64
	baseGamePath /= "game.exe";
	resultPath /= TextFormat("%s.exe", projectTitle.c_str());
#else
	baseGamePath /= "game";
	resultPath /= projectTitle;
#endif

	std::filesystem::copy(baseGamePath, resultPath,
						  std::filesystem::copy_options::overwrite_existing);

#ifdef _WIN64

	HANDLE outFile = nullptr;

	outFile = CreateFile("build.log", GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
						 OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdOutput = outFile;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bool success =
		CreateProcess(NULL, resultPath.string().data(), NULL, NULL, true, 0,
					  NULL, NULL, &siStartInfo, &piProcInfo);

	if (!success) {
		printf("Child process could not be created.. \n");
	} else {
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		CloseHandle(outFile);
	}

#else

	pid_t pid = fork();

	if (pid == 0) {
		int fd = open("log.log", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		dup2(fd, STDOUT_FILENO);

		execl(resultPath.c_str(), NULL);
	} else if (pid > 0) {
		printf("Started the game.. \n");
		wait(0);
	} else {
		printf("Error while forking..\n");
	}

#endif
}
