#include "childWindows/addDialogueOptionWindow.hpp"

#include <memory>

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/textField.hpp"

AddDialogueOptionWindow::AddDialogueOptionWindow() : PopupWindow("Add Dialogue Option..") {
	bindTranslation(this->currentWindow, "dialog.add_dialogue_option.title", &tgui::ChildWindow::setTitle);

	dialogue = nullptr;
	lineIndex = 0;
	fileView = nullptr;

	currentWindow->setSize(280, 180);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({4, 4});

	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4);
	panel->add(layout);

	auto nameField = TextField::create();
	nameField->setSize({"100%", 24});
	bindTranslation<tgui::Label>(nameField->label, "dialog.add_dialogue_option.name", &tgui::Label::setText);
	layout->add(nameField);

	auto dialogueField = FileField::create();
	dialogueField->setSize({"100%", 24});
	bindTranslation<tgui::Label>(dialogueField->label, "dialog.add_dialogue_option.dialogue", &tgui::Label::setText);
	dialogueField->pathFilters = {{"RPG++ Dialogue", {"*.rdiag"}}};
	layout->add(dialogueField);

	auto confirmButton = tgui::Button::create();
	bindTranslation<tgui::Button>(confirmButton, "dialog.add_dialogue_option.submit", &tgui::Button::setText);
	confirmButton->setSize({"100%", 24});

	std::weak_ptr<TextField> weakName = nameField;
	std::weak_ptr<FileField> weakDialogue = dialogueField;
	confirmButton->onClick([weakName, weakDialogue, this] {
		if (weakName.expired() || weakDialogue.expired()) return;

		auto nameShared = weakName.lock();
		auto dialogueShared = weakDialogue.lock();

		auto nameText = nameShared->value->getText().toStdString();

		if (!nameText.empty() && !dialogueShared->value->getText().empty()) {
			dialogue->getData().lines[lineIndex].hasOptions = true;
			dialogue->getData().lines[lineIndex].options.push_back(
				{nameText, dialogueShared->value->getText().toStdString()});

			nameShared->value->setText("");
			dialogueShared->value->setText("");

			fileView->initOptionPanel(lineIndex);

			close();
		}
	});

	layout->add(confirmButton);

	currentWindow->add(panel);
}
