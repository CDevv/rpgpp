#include "fileSystemService.hpp"
#include <array>
#include <nfd.h>
#include <nfd.hpp>
#include <string>
#include "TGUI/Widgets/FileDialog.hpp"

#include "editor.hpp"
#include "projectScreen.hpp"

FileSystemService::FileSystemService() {
	typeNames[static_cast<int>(EngineFileType::FILE_TILESET)] = "Tilesets";
	typeNames[static_cast<int>(EngineFileType::FILE_MAP)] = "Maps";

    NFD_Init();
}

void FileSystemService::unload() {
    NFD_Quit();
}

void FileSystemService::promptOpenProject() {
    auto files = tgui::FileDialog::create();
    files->setFileTypeFilters({ { "RPG++ Project", {"*.rpgpp"} }});

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