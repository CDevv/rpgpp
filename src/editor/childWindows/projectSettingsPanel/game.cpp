#include "childWindows/projectSettingsPanel/game.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "project.hpp"
#include "raylib.h"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"

ProjectSettingsPanelGame::ProjectSettingsPanelGame(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "dialog.project_settings.game._label") {
	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	defaultRoom = FileField::create();
	bindTranslation(defaultRoom->label, "dialog.project_settings.game.default_room", &tgui::Label::setText);
	defaultRoom->setSize({"100%", 24});
	defaultRoom->pathFilters = {{"RPG++ Room", {"*.rmap"}}};
	defaultRoom->callback = [](const tgui::String &path) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getGameSettings().defaultRoomPath = TextFormat("maps/%s", GetFileName(path.toStdString().c_str()));
		}
	};

	playerActor = FileField::create();
	bindTranslation(playerActor->label, "dialog.project_settings.game.player_actor", &tgui::Label::setText);
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
	bindTranslation(tileSize->label, "dialog.project_settings.game.tile_size", &tgui::Label::setText);
	tileSize->setSize({"100%", 24});
	tileSize->label->setText("Tile Size");
	tileSize->value->setMinimum(16);
	tileSize->value->setMaximum(64);
	tileSize->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getGameSettings().tileSize = value;
		}
	});

	debugDraw = BoolField::create();
	bindTranslation(debugDraw->label, "dialog.project_settings.game.debug_draw", &tgui::Label::setText);
	debugDraw->label->setText("Debug Draw");
	debugDraw->setSize({"100%", 24});
	debugDraw->value->onCheck([](bool value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getGameSettings().debugDraw = value;
		}
	});

	layout->add(defaultRoom);
	layout->add(playerActor);
	layout->add(tileSize);
	layout->add(debugDraw);

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