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
	: SettingsPanelBase(tabContainer, "dialog.project_settings.program._label") {
	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	titleField = TextField::create();
	bindTranslation(titleField->label, "dialog.project_settings.program.title", &tgui::Label::setText);
	titleField->setSize({"100%", 24});
	titleField->value->onTextChange([](const tgui::String &text) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getProgramSettings().projectTitle = text.toStdString();
		}
	});

	versionField = TextField::create();
	bindTranslation(versionField->label, "dialog.project_settings.program.version", &tgui::Label::setText);
	versionField->setSize({"100%", 24});
	versionField->value->onTextChange([](const tgui::String &text) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getProgramSettings().projectVersion = text.toStdString();
		}
	});

	programIcon = FileField::create();
	programIcon->setSize({"100%", 24});
	programIcon->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	bindTranslation(programIcon->label, "dialog.project_settings.program.program_icon", &tgui::Label::setText);
	programIcon->callback = [](const tgui::String &path) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getProgramSettings().programIconPath =
				TextFormat("images/%s", GetFileName(path.toStdString().c_str()));
		}
	};

	windowSizeX = IntField::create();
	windowSizeX->setSize({"100%", 24});
	bindTranslation(windowSizeX->label, "dialog.project_settings.program.window_width", &tgui::Label::setText);
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
	bindTranslation(windowSizeY->label, "dialog.project_settings.program.window_height", &tgui::Label::setText);
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
	bindTranslation(resizeable->label, "dialog.project_settings.program.is_resizable", &tgui::Label::setText);
	resizeable->value->onChange([](bool value) {
		Project *project = Editor::instance->getProject();
		project->getProgramSettings().windowResizeableFlag = value;
	});

	windowStateFlag = SelectField::create();
	windowStateFlag->setSize({"100%", 24});
	bindTranslation(windowStateFlag->label, "dialog.project_settings.program.window_mode", &tgui::Label::setText);
	for (auto &[k, v] : WindowStateToName) {
		windowStateFlag->value->addItem(v);
	}
	windowStateFlag->value->onItemSelect([](const tgui::String &text) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			for (auto &[k, v] : WindowStateToName) {
				if (v == text) {
					project->getProgramSettings().windowStateFlag = k;
					break;
				}
			}
		}
	});

	targetFPS = IntField::create();
	targetFPS->setSize({"100%", 24});
	bindTranslation(targetFPS->label, "dialog.project_settings.program.target_fps", &tgui::Label::setText);
	targetFPS->value->setMinimum(0);
	targetFPS->value->setMaximum(1000);
	targetFPS->value->onValueChange([](int value) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->getProgramSettings().targetFPS = value;
		}
	});

	layout->add(titleField);
	layout->add(programIcon);
	layout->add(windowSizeX);
	layout->add(windowSizeY);
	layout->add(resizeable);
	layout->add(windowStateFlag);
	layout->add(targetFPS);

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

	versionField->value->setText(programSet.projectVersion);
	windowSizeX->value->setValue(programSet.windowSize.x);
	windowSizeY->value->setValue(programSet.windowSize.y);

	resizeable->value->setChecked(programSet.windowResizeableFlag);
	windowStateFlag->value->setSelectedItem(WindowStateToName.at(programSet.windowStateFlag));
	targetFPS->value->setValue(programSet.targetFPS);
}
