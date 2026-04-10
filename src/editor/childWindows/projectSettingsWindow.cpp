#include "childWindows/projectSettingsWindow.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "editor.hpp"
#include "project.hpp"
#include "widgets/propertyFields/textField.hpp"

ProjectSettingsWindow::ProjectSettingsWindow() : PopupWindow("ProjectSettings") {
	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setPadding(4);
	layout->getRenderer()->setSpaceBetweenWidgets(4.0f);

	bindTranslation(this->currentWindow,
									   "dialog.project_settings._label",
									   &tgui::ChildWindow::setTitle);

	titleField = TextField::create();
	bindTranslation(titleField->label, "dialog.project_settings.title", &tgui::Label::setText);
	titleField->setPosition({8, 48});
	titleField->setSize({"80%", 24});
	titleField->value->onTextChange([](const tgui::String &text) {
		Project *project = Editor::instance->getProject();
		if (project != nullptr) {
			project->setTitle(text.toStdString());
		}
	});

	layout->add(titleField);

	this->currentWindow->add(layout);
}

void ProjectSettingsWindow::open() {
	Project *project = Editor::instance->getProject();

	titleField->value->setText(project->getTitle());

	PopupWindow::open();
}
