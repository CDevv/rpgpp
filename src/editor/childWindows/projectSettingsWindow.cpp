#include "childWindows/projectSettingsWindow.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "childWindows/popupWindow.hpp"
#include "editor.hpp"
#include "project.hpp"
#include "widgets/propertyFields/textField.hpp"

ProjectSettingsWindow::ProjectSettingsWindow() : PopupWindow("ProjectSettings") {
	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4.0f);

	auto titleLabel = tgui::Label::create();
	titleLabel->setSize({"80%", 36});
	titleLabel->setText("Project Settings");

	layout->add(titleLabel);

	titleField = TextField::create();
	titleField->label->setText("Title");
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