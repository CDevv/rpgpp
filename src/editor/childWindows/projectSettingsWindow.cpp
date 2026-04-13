#include "childWindows/projectSettingsWindow.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/projectSettingsPanel/game.hpp"
#include "childWindows/projectSettingsPanel/program.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "editor.hpp"
#include "project.hpp"
#include "raylib.h"
#include "widgets/propertyFields/textField.hpp"

ProjectSettingsWindow::ProjectSettingsWindow() : PopupWindow("ProjectSettings") {
	tgui::TabContainer::Ptr tabContainer = tgui::TabContainer::create();
	tabContainer->setPosition(0, 0);
	tabContainer->setSize({"100%", "100% - 36"});

	auto saveButton = tgui::Button::create("Save");
	saveButton->setPosition(2, "100% - 32");
	saveButton->setSize("100% - 4", 30);
	saveButton->onClick([] {
		Project *project = Editor::instance->getProject();

		auto projectJson = project->toJson();
		auto basePath = project->getBasePath();

		SaveFileText(TextFormat("%s/proj.rpgpp", basePath.c_str()), projectJson.dump().c_str());
	});

	bindTranslation(this->currentWindow, "dialog.project_settings._label", &tgui::ChildWindow::setTitle);

	program = std::make_unique<ProjectSettingsPanelProgram>(tabContainer);
	game = std::make_unique<ProjectSettingsPanelGame>(tabContainer);

	tabContainer->getTabs()->select(0);

	this->currentWindow->add(tabContainer);
	this->currentWindow->add(saveButton);
}

void ProjectSettingsWindow::open() {
	Project *project = Editor::instance->getProject();

	program->setup(project);
	game->setup(project);

	PopupWindow::open();
}
