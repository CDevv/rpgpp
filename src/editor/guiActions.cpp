#include "guiActions.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include "editor.hpp"
#include "projectScreen.hpp"

void GUIActions::openProject() {
	auto files = tgui::FileDialog::create();

	files->onFileSelect([](const tgui::String &filePath) {
		Editor::instance->setProject(filePath.toStdString());
		Editor::instance->getGui().setScreen(
			std::make_unique<screens::ProjectScreen>(), false);
	});

	Editor::instance->getGui().gui->add(files);
}