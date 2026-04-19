#include "childWindows/addDelayDialogueWindow.hpp"
#include <memory>
#include <string>
#include "TGUI/Layout.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"

AddDelayDialogueWindow::AddDelayDialogueWindow(): PopupWindow("Add Delay...") {
	this->editor = nullptr;
	bindTranslation<tgui::ChildWindow>(this->currentWindow, "screen.project.dialogueview.delaywindow.add_a_delay", &tgui::ChildWindow::setTitle);

	this->currentWindow->setSize(400, 150);

	auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->setOrigin({0.5, 0.5});
	verticalLayout->setPosition({"50%", "50%"});
	verticalLayout->getRenderer()->setPadding(5.0f);

	auto descriptionLabel = tgui::Label::create();
	descriptionLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	bindTranslation<tgui::Label>(descriptionLabel, "screen.project.dialogueview.delaywindow.description", &tgui::Label::setText);
	verticalLayout->add(descriptionLabel);

	// Allow for more precise second editing.
	this->delaySpin = tgui::SpinControl::create();
	delaySpin->setSize({"100%", 32});
	delaySpin->setStep(0.1);
	delaySpin->setDecimalPlaces(3);
	verticalLayout->add(this->delaySpin);

	auto confirmButton = tgui::Button::create();
	bindTranslation<tgui::Button>(confirmButton, "screen.project.dialogueview.delaywindow.confirm", &tgui::Button::setText);
	confirmButton->onPress.connect([this]{
		this->editor->addXmlTagWithProperties("delay", {{"delay", std::to_string(delaySpin->getValue())}});
		this->currentWindow->close();
	});
	verticalLayout->add(confirmButton);

	currentWindow->add(verticalLayout);
}

void AddDelayDialogueWindow::open(std::shared_ptr<DialogueEditor> editor) {
	this->editor = editor;
	PopupWindow::open();
}
