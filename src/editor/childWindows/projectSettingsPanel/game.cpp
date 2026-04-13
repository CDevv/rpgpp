#include "childWindows/projectSettingsPanel/game.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "project.hpp"
#include "raylib.h"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"

ProjectSettingsPanelGame::ProjectSettingsPanelGame(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "screen.project_options.game._label") {
	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	defaultRoom = FileField::create();
	defaultRoom->setSize({"100%", 24});
	defaultRoom->pathFilters = {{"RPG++ Room", {"*.rmap"}}};
	defaultRoom->label->setText("Default Room");
	defaultRoom->callback = [](const tgui::String &path) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getGameSettings().defaultRoomPath =
				TextFormat("rooms/%s", GetFileName(path.toStdString().c_str()));
		}
	};

	playerActor = FileField::create();
	playerActor->setSize({"100%", 24});
	playerActor->pathFilters = {{"RPG++ Actor", {"*.ractor"}}};
	playerActor->label->setText("Player Actor");
	playerActor->callback = [](const tgui::String &path) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getGameSettings().playerActorPath =
				TextFormat("actors/%s", GetFileName(path.toStdString().c_str()));
		}
	};

	tileSize = IntField::create();
	tileSize->setSize({"100%", 24});
	tileSize->value->setMinimum(16);
	tileSize->value->setMaximum(64);
	tileSize->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getGameSettings().tileSize = value;
		}
	});

	layout->add(defaultRoom);
	layout->add(playerActor);
	layout->add(tileSize);

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}

void ProjectSettingsPanelGame::setup(Project *project) {
	if (project == nullptr) return;

	auto &gameSet = project->getGameSettings();

	defaultRoom->value->setText(GetFileName(gameSet.defaultRoomPath.c_str()));
	playerActor->value->setText(GetFileName(gameSet.playerActorPath.c_str()));
	tileSize->value->setValue(gameSet.tileSize);
}