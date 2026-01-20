#include "fileSystemService.hpp"
#include <nfd.h>
#include <nfd.hpp>
#include "TGUI/Widgets/FileDialog.hpp"

#include "editor.hpp"
#include "projectScreen.hpp"

FileSystemService::FileSystemService() {
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