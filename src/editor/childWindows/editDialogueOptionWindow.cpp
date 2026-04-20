#include "childWindows/editDialogueOptionWindow.hpp"

#include "TGUI/Widgets/Button.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"

EditDialogueOptionWindow::EditDialogueOptionWindow() : PopupWindow("Edit a Dialogue Option..") {
	bindTranslation(this->currentWindow, "dialog.edit_dialogue_option.title", &tgui::ChildWindow::setTitle);

	dialogue = nullptr;
	lineIndex = 0;
	fileView = nullptr;
	optionIndex = 0;

	currentWindow->setSize(280, 180);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({4, 4});

	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4);
	panel->add(layout);

	nameField = TextField::create();
	nameField->setSize({"100%", 24});
	bindTranslation<tgui::Label>(nameField->label, "dialog.edit_dialogue_option.name", &tgui::Label::setText);
	layout->add(nameField);

	dialogueField = FileField::create();
	dialogueField->setSize({"100%", 24});
	bindTranslation<tgui::Label>(dialogueField->label, "dialog.edit_dialogue_option.dialogue", &tgui::Label::setText);
	dialogueField->pathFilters = {{"RPG++ Dialogue", {"*.rdiag"}}};
	layout->add(dialogueField);

	auto confirmButton = tgui::Button::create();
	bindTranslation<tgui::Button>(confirmButton, "dialog.edit_dialogue_option.submit", &tgui::Button::setText);
	confirmButton->setSize({"100%", 24});

	std::weak_ptr<TextField> weakName = nameField;
	std::weak_ptr<FileField> weakDialogue = dialogueField;
	confirmButton->onClick([weakName, weakDialogue, this] {
		if (weakName.expired() || weakDialogue.expired()) return;

		auto nameShared = weakName.lock();
		auto dialogueShared = weakDialogue.lock();

		auto nameText = nameShared->value->getText().toStdString();

		auto &line = dialogue->getData().lines.at(lineIndex);
		auto &option = line.options.at(optionIndex);

		option.title = nameText;
		option.nextDialogue = dialogueShared->value->getText().toStdString();

		fileView->initOptionPanel(lineIndex);

		close();
	});
	layout->add(confirmButton);

	auto removeButton = tgui::Button::create();
	bindTranslation<tgui::Button>(removeButton, "dialog.edit_dialogue_option.remove", &tgui::Button::setText);
	removeButton->setSize({"100%", 24});
	removeButton->onClick([this] {
		auto &line = dialogue->getData().lines.at(lineIndex);
		line.options.erase(line.options.begin() + optionIndex);

		fileView->initOptionPanel(lineIndex);

		close();
	});
	layout->add(removeButton);

	currentWindow->add(panel);
}

void EditDialogueOptionWindow::setup(Dialogue *dialogue, int lineIndex, int optionIndex) {
	this->dialogue = dialogue;
	this->lineIndex = lineIndex;
	this->optionIndex = optionIndex;

	auto &line = dialogue->getData().lines.at(lineIndex);
	auto &option = line.options.at(optionIndex);

	nameField->value->setText(option.title);
	dialogueField->value->setText(option.nextDialogue);
}