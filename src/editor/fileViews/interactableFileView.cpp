#include "fileViews/interactableFileView.hpp"

#include "TGUI/String.hpp"
#include "interactable.hpp"
#include "raylib.h"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/textField.hpp"

InteractableFileView::InteractableFileView() {
	auto props = PropertiesBox::create();
	props->setSize({RIGHT_PANEL_W, "100%"});
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	nameField = TextField::create();
	nameField->label->setText("Display Name");
	nameField->value->onTextChange([this](const tgui::String &text) {
		if (variant != nullptr) {
			const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
			const auto interactable = ptr->get();

			interactable->setDisplayTitle(text.toStdString());
		}
	});
	props->addTextField(nameField);

	scriptField = FileField::create();
	scriptField->pathFilters = {{"Script", {"*.lua"}}};
	scriptField->label->setText("Script");
	scriptField->callback = [this](const tgui::String &path) {
		if (variant != nullptr) {
			const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
			const auto interactable = ptr->get();

			interactable->setScriptSourcePath(TextFormat("scripts/%s", GetFileName(path.toStdString().c_str())));
		}
	};
	props->addFileField(scriptField);

	onTouchField = BoolField::create();
	onTouchField->label->setText("On Touch?");
	onTouchField->value->onChange([this](bool value) {
		if (variant != nullptr) {
			const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
			const auto interactable = ptr->get();

			interactable->setOnTouch(value);
		}
	});
	props->addBooleanField(onTouchField);

	widgetContainer.push_back(props);
}

void InteractableFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
		const auto interactable = ptr->get();

		nameField->value->setText(interactable->getDisplayTitle());
		scriptField->value->setText(GetFileName(interactable->getScriptSourcePath().c_str()));
		onTouchField->value->setChecked(interactable->isOnTouch());

		addWidgets(layout);
	}
}