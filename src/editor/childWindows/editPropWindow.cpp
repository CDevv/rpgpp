#include "childWindows/editPropWindow.hpp"

#include <memory>

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "childWindows/newPropWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

EditPropWindow::EditPropWindow() : PopupWindow("Edit Prop..") {
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

	dropdown = tgui::ComboBox::create();
	dropdown->setSize({"100%", 24});
	dropdown->addMultipleItems({"integer", "string", "boolean", "dialogue"});
	dropdown->setSelectedItemByIndex(0);
	layout->add(dropdown);

	auto confirmButton = tgui::Button::create("Submit");
	confirmButton->setSize({"100%", 24});

	std::weak_ptr<tgui::ComboBox> weakDropdown = dropdown;
	confirmButton->onClick([this, weakDropdown] {
		if (weakDropdown.expired()) {
			return;
		}

		auto dropdownShared = weakDropdown.lock();

		std::string nameText = propName;

		if (interactable != nullptr) {
			int idx = dropdownShared->getSelectedItemIndex();
			PropType propType = static_cast<PropType>(idx);

			interactable->getProps().erase(nameText);

			interactable->addProp(propType, nameText);

			box->addPropsJson(interactable->getProps(), true, true);
			close();
		}
	});
	layout->add(confirmButton);

	auto removeButton = tgui::Button::create("Remove");
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