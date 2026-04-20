#include "childWindows/projectSettingsPanel/game.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/editListFieldWindow.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "editor.hpp"
#include "listHelper.hpp"
#include "project.hpp"
#include "raylib.h"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/listField.hpp"

ProjectSettingsPanelGame::ProjectSettingsPanelGame(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "dialog.project_settings.game._label") {
	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	editListFieldWindow = std::make_unique<EditListFieldWindow<int>>();

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

	exportImageScales = ListField<int>::create();
	bindTranslation(exportImageScales->label, "dialog.project_settings.game.export_image_scales",
					&tgui::Label::setText);
	exportImageScales->label->setText("Export Image Scales");
	exportImageScales->setSize({"100%", 24});
	exportImageScales->value->onPress([this] {
		Project *project = Editor::instance->getProject();

		if (project != nullptr) {
			editListFieldWindow->field = exportImageScales.get();
			editListFieldWindow->setup(&project->getGameSettings().exportImageScales);
			editListFieldWindow->open();
		}
	});

	exportFontSizes = ListField<int>::create();
	bindTranslation(exportFontSizes->label, "dialog.project_settings.game.export_font_sizes", &tgui::Label::setText);
	exportFontSizes->label->setText("Export Font Sizes");
	exportFontSizes->setSize({"100%", 24});
	exportFontSizes->value->onPress([this] {
		Project *project = Editor::instance->getProject();

		if (project != nullptr) {
			editListFieldWindow->field = exportFontSizes.get();
			editListFieldWindow->setup(&project->getGameSettings().exportFontSizes);
			editListFieldWindow->open();
		}
	});

	layout->add(defaultRoom);
	layout->add(playerActor);
	layout->add(tileSize);
	layout->add(debugDraw);
	layout->add(exportImageScales);
	layout->add(exportFontSizes);

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}

void ProjectSettingsPanelGame::setup(Project *project) {
	if (project == nullptr) return;

	auto &gameSet = project->getGameSettings();

	defaultRoom->value->setText(GetFileName(gameSet.defaultRoomPath.c_str()));
	playerActor->value->setText(GetFileName(gameSet.playerActorPath.c_str()));
	tileSize->value->setValue(gameSet.tileSize);
	exportImageScales->value->setText(VecToString(gameSet.exportImageScales));
	exportFontSizes->value->setText(VecToString(gameSet.exportFontSizes));
}