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
			project->getProgramSettings().projectTitle = text.toStdString();
		}
	});

	programIcon = FileField::create();
	programIcon->setSize({"100%", 24});
	programIcon->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	programIcon->label->setText("Program Icon");
	programIcon->callback = [](const tgui::String &path) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getProgramSettings().programIconPath =
				TextFormat("images/%s", GetFileName(path.toStdString().c_str()));
		}
	};

	windowSizeX = IntField::create();
	windowSizeX->setSize({"100%", 24});
	windowSizeX->label->setText("Window Width");
	windowSizeX->value->setMinimum(640);
	windowSizeX->value->setMaximum(1920);
	windowSizeX->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			auto windowSize = project->getProgramSettings().windowSize;
			windowSize.x = value;
			project->getProgramSettings().windowSize = windowSize;
		}
	});

	windowSizeY = IntField::create();
	windowSizeY->setSize({"100%", 24});
	windowSizeY->label->setText("Window Height");
	windowSizeY->value->setMinimum(480);
	windowSizeY->value->setMaximum(1080);
	windowSizeY->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			auto windowSize = project->getProgramSettings().windowSize;
			windowSize.y = value;
			project->getProgramSettings().windowSize = windowSize;
		}
	});

	resizeable = BoolField::create();
	resizeable->setSize({"100%", 24});
	resizeable->label->setText("Window Resizeable?");
	resizeable->value->onChange([](bool value) {
		Project *project = Editor::instance->getProject();
		project->getProgramSettings().windowResizeableFlag = value;
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

	auto &programSet = project->getProgramSettings();

	titleField->value->setText(programSet.projectTitle);
	if (!programSet.programIconPath.empty()) {
		programIcon->value->setText(GetFileName(programSet.programIconPath.c_str()));
	}

	windowSizeX->value->setValue(programSet.windowSize.x);
	windowSizeY->value->setValue(programSet.windowSize.y);

	resizeable->value->setChecked(programSet.windowResizeableFlag);
}