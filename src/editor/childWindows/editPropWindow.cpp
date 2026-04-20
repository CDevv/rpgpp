#include "childWindows/editPropWindow.hpp"

#include <memory>

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "bindTranslation.hpp"
#include "childWindows/newPropWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "widgets/propertyFields/selectField.hpp"

EditPropWindow::EditPropWindow() : PopupWindow("Edit Prop..") {
	bindTranslation(this->currentWindow, "dialog.edit_prop.title", &tgui::ChildWindow::setTitle);
	props = nullptr;
	currentWindow->setSize(280, 180);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({4, 4});

	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4);
	panel->add(layout);

	nameLabel = tgui::Label::create();
	nameLabel->setSize({"100%", 24});
	nameLabel->setText(propName);
	layout->add(nameLabel);

	dropdown = SelectField::create();
	dropdown->setSize({"100%", 24});
	bindTranslation<tgui::Label>(dropdown->label, "dialog.edit_prop.type", &tgui::Label::setText);
	dropdown->value->addMultipleItems({"integer", "string", "boolean", "dialogue"});
	dropdown->value->setSelectedItemByIndex(0);
	layout->add(dropdown);

	auto confirmButton = tgui::Button::create();
	bindTranslation<tgui::Button>(confirmButton, "dialog.edit_prop.submit", &tgui::Button::setText);
	confirmButton->setSize({"100%", 24});

	std::weak_ptr<SelectField> weakDropdown = dropdown;
	confirmButton->onClick([this, weakDropdown] {
		if (weakDropdown.expired()) {
			return;
		}

		auto dropdownShared = weakDropdown.lock();

		std::string nameText = propName;

		if (interactable != nullptr) {
			int idx = dropdownShared->value->getSelectedItemIndex();
			PropType propType = static_cast<PropType>(idx);

			interactable->getProps().erase(nameText);

			interactable->addProp(propType, nameText);

			box->addPropsJson(interactable->getProps(), true, true);
			close();
		}
	});
	layout->add(confirmButton);

	auto removeButton = tgui::Button::create();
	bindTranslation<tgui::Button>(removeButton, "dialog.edit_prop.remove", &tgui::Button::setText);
	removeButton->setSize({"100%", 24});
	removeButton->onClick([this] {
		if (interactable != nullptr) {
			interactable->getProps().erase(propName);

			box->addPropsJson(*interactable->getPropsPtr(), true, true);
			close();
		}
	});
	layout->add(removeButton);

	currentWindow->add(panel);
}
