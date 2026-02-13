#include "services/fileSystemService.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include <array>
#include <nfd.h>
#include <nfd.hpp>
#include <string>

#include "editor.hpp"
#include "screens/projectScreen.hpp"
#include "raylib.h"

FileSystemService::FileSystemService() {
	editorBaseDir = GetWorkingDirectory();

	typeNames[static_cast<int>(EngineFileType::FILE_TILESET)] = "Tilesets";
	typeNames[static_cast<int>(EngineFileType::FILE_MAP)] = "Maps";
	typeNames[static_cast<int>(EngineFileType::FILE_ACTOR)] = "Actors";
	typeNames[static_cast<int>(EngineFileType::FILE_DIALOGUE)] = "Dialogues";
	typeNames[static_cast<int>(EngineFileType::FILE_IMAGE)] = "Images";
	typeNames[static_cast<int>(EngineFileType::FILE_FONT)] = "Fonts";
	typeNames[static_cast<int>(EngineFileType::FILE_SOUND)] = "Sounds";
	typeNames[static_cast<int>(EngineFileType::FILE_MUSIC)] = "Music";
	typeNames[static_cast<int>(EngineFileType::FILE_PROP)] = "Props";
	typeNames[static_cast<int>(EngineFileType::FILE_SCRIPT)] = "Scripts";

	NFD_Init();
}

void FileSystemService::unload() { NFD_Quit(); }

void FileSystemService::promptOpenProject() {
	auto files = tgui::FileDialog::create();
	files->setFileTypeFilters({{"RPG++ Project", {"*.rpgpp"}}});

	files->onFileSelect([](const tgui::String &filePath) {
		Editor::instance->setProject(filePath.toStdString());
		Editor::instance->getGui().setScreen(
			std::make_unique<screens::ProjectScreen>(), false);
	});

	Editor::instance->getGui().gui->add(files);
}

std::string &FileSystemService::getTypeName(EngineFileType fileType) {
	return typeNames[static_cast<int>(fileType)];
}

std::string &FileSystemService::getEditorBaseDir() { return editorBaseDir; }

std::string FileSystemService::getResourcePath(const std::string &path) {
	std::filesystem::path result = editorBaseDir;
	result /= "resources";
	result /= path;

	return result.string();
}
