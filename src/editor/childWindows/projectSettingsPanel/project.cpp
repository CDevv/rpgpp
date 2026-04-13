#include "project.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/projectSettingsPanel/program.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "raylib.h"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/textField.hpp"

ProjectSettingsPanelProgram::ProjectSettingsPanelProgram(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "screen.project_options.program._label") {
	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	titleField = TextField::create();
	bindTranslation(titleField->label, "dialog.project_settings.title", &tgui::Label::setText);
	titleField->setSize({"100%", 24});
	titleField->value->onTextChange([](const tgui::String &text) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->setTitle(text.toStdString());
		}
	});

	programIcon = FileField::create();
	programIcon->setSize({"100%", 24});
	programIcon->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	bindTranslation(programIcon->label, "dialog.project_settings.program_icon", &tgui::Label::setText);
	programIcon->callback = [](const tgui::String &path) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->setProgramIconPath(TextFormat("images/%s", GetFileName(path.toStdString().c_str())));
		}
	};

	windowSizeX = IntField::create();
	windowSizeX->setSize({"100%", 24});
	bindTranslation(windowSizeX->label, "dialog.project_settings.window_width", &tgui::Label::setText);
	windowSizeX->value->setMinimum(640);
	windowSizeX->value->setMaximum(1920);
	windowSizeX->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			auto windowSize = project->getWindowSize();
			windowSize.x = value;
			project->setWindowSize(windowSize);
		}
	});

	windowSizeY = IntField::create();
	windowSizeY->setSize({"100%", 24});
	bindTranslation(windowSizeY->label, "dialog.project_settings.window_height", &tgui::Label::setText);
	windowSizeY->value->setMinimum(480);
	windowSizeY->value->setMaximum(1080);
	windowSizeY->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			auto windowSize = project->getWindowSize();
			windowSize.y = value;
			project->setWindowSize(windowSize);
		}
	});

	resizeable = BoolField::create();
	resizeable->setSize({"100%", 24});
	bindTranslation(resizeable->label, "dialog.project_settings.is_resizable", &tgui::Label::setText);
	resizeable->value->onChange([](bool value) {
		Project *project = Editor::instance->getProject();
		project->setIsWindowResizeable(value);
	});

	layout->add(titleField);
	layout->add(programIcon);
	layout->add(windowSizeX);
	layout->add(windowSizeY);
	layout->add(resizeable);

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}

void ProjectSettingsPanelProgram::setup(Project *project) {
	if (project == nullptr) return;

	titleField->value->setText(project->getTitle());
	if (!project->getProgramIconPath().empty()) {
		programIcon->value->setText(GetFileName(project->getProgramIconPath().c_str()));
	}

	windowSizeX->value->setValue(project->getWindowSize().x);
	windowSizeY->value->setValue(project->getWindowSize().y);
}
