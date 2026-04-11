#include "childWindows/newPropWindow.hpp"

#include <memory>

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "childWindows/popupWindow.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "widgets/propertyFields/textField.hpp"

NewPropWindow::NewPropWindow() : PopupWindow("New Prop..") {
	props = nullptr;
	currentWindow->setSize(280, 180);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({4, 4});

	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4);
	panel->add(layout);

	auto nameField = TextField::create();
	nameField->setSize({"100%", 24});
	nameField->label->setText("Name");
	layout->add(nameField);

	auto dropdown = tgui::ComboBox::create();
	dropdown->setSize({"100%", 24});
	dropdown->addMultipleItems({"integer", "string", "boolean", "dialogue"});
	dropdown->setSelectedItemByIndex(0);
	layout->add(dropdown);

	auto confirmButton = tgui::Button::create("Submit");
	confirmButton->setSize({"100%", 24});

	std::weak_ptr<TextField> weakName = nameField;
	std::weak_ptr<tgui::ComboBox> weakDropdown = dropdown;
	confirmButton->onClick([this, weakName, weakDropdown] {
		if (weakName.expired() || weakDropdown.expired()) {
			return;
		}

		auto nameShared = weakName.lock();
		auto dropdownShared = weakDropdown.lock();

		auto nameText = nameShared->value->getText().toStdString();

		if (interactable != nullptr && !nameText.empty()) {
			int idx = dropdownShared->getSelectedItemIndex();
			PropType propType = static_cast<PropType>(idx);

			interactable->addProp(propType, nameText);

			box->addPropsJson(interactable->getProps(), true, true);
			close();
		}
	});
	layout->add(confirmButton);

	currentWindow->add(panel);
}