#include "childWindows/newPropWindow.hpp"

#include <memory>

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"

NewPropWindow::NewPropWindow() : PopupWindow("New Prop..") {
	bindTranslation(this->currentWindow, "dialog.new_prop.title", &tgui::ChildWindow::setTitle);
	props = nullptr;
	currentWindow->setSize(280, 180);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({4, 4});

	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4);
	panel->add(layout);

	auto nameField = TextField::create();
	nameField->setSize({"100%", 24});
	bindTranslation<tgui::Label>(nameField->label, "dialog.new_prop.name", &tgui::Label::setText);
	layout->add(nameField);

	auto dropdownField = SelectField::create();
	dropdownField->setSize({"100%", 24});
	bindTranslation<tgui::Label>(dropdownField->label, "dialog.new_prop.type", &tgui::Label::setText);
	dropdownField->value->addMultipleItems({"integer", "string", "boolean", "dialogue"});
	dropdownField->value->setSelectedItemByIndex(0);
	layout->add(dropdownField);

	auto confirmButton = tgui::Button::create();
	bindTranslation<tgui::Button>(confirmButton, "dialog.new_prop.submit", &tgui::Button::setText);
	confirmButton->setSize({"100%", 24});

	std::weak_ptr<TextField> weakName = nameField;
	std::weak_ptr<SelectField> weakDropdown = dropdownField;
	confirmButton->onClick([this, weakName, weakDropdown] {
		if (weakName.expired() || weakDropdown.expired()) {
			return;
		}

		auto nameShared = weakName.lock();
		auto dropdownShared = weakDropdown.lock();

		auto nameText = nameShared->value->getText().toStdString();

		if (interactable != nullptr && !nameText.empty()) {
			int idx = dropdownShared->value->getSelectedItemIndex();
			PropType propType = static_cast<PropType>(idx);

			interactable->addProp(propType, nameText);

			box->addPropsJson(interactable->getProps(), true, true);
			close();
		}

		nameShared->value->setText("");
		dropdownShared->value->setSelectedItemByIndex(0);
	});
	layout->add(confirmButton);

	currentWindow->add(panel);
}
