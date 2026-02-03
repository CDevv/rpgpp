#include "project.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "projectScreen.hpp"
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