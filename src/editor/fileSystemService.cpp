#include "fileSystemService.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include <array>
#include <nfd.h>
#include <nfd.hpp>
#include <string>
#include <typeindex>

#include "editor.hpp"
#include "fileViews/tilesetFileView.hpp"
#include "projectScreen.hpp"
#include "raylib.h"

FileSystemService::FileSystemService() {
	editorBaseDir = GetWorkingDirectory();

	typeNames[static_cast<int>(EngineFileType::FILE_TILESET)] = "Tilesets";
	typeNames[static_cast<int>(EngineFileType::FILE_MAP)] = "Maps";

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

std::string FileSystemService::getResourcePath(const std::string &path) {
	std::filesystem::path result = editorBaseDir;
	result /= "resources";
	result /= path;

	return result.string();
}