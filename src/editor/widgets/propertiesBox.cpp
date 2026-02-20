#include "widgets/propertiesBox.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include "widgets/textField.hpp"
#include <memory>
#include <nlohmann/json.hpp>

PropertiesBox::PropertiesBox(const char *typeName, bool initRenderer)
	: tgui::ChildWindow(typeName, initRenderer) {
	this->setTitle("Props");
	this->setTitleButtons(tgui::ChildWindow::TitleButton::None);
	auto vertLayout = tgui::GrowVerticalLayout::create();
	vertLayout->getRenderer()->setSpaceBetweenWidgets(GAP);
	add(vertLayout);
	this->layout = vertLayout;
}

PropertiesBox::Ptr PropertiesBox::create() {
	return std::make_shared<PropertiesBox>();
}

PropertiesBox::Ptr PropertiesBox::copy(PropertiesBox::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<PropertiesBox>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr PropertiesBox::clone() const {
	return std::make_shared<PropertiesBox>(*this);
}

void PropertiesBox::draw(tgui::BackendRenderTarget &target,
						 tgui::RenderStates states) const {
	tgui::ChildWindow::draw(target, states);
}

void PropertiesBox::addPropsJson(nlohmann::json &j) {
	for (auto item : j.items()) {
		printf("%s \n", item.key().c_str());
		if (item.value().is_string()) {
			printf("%s \n", item.value().get<std::string>().c_str());

			auto textField = TextField::create();
			textField->label->setText(item.key());
			textField->value->setText(item.value().get<std::string>());
			textField->value->onTextChange(
				[&j, item](const tgui::String &text) {
					std::string st = text.toStdString();
					j.at(item.key()) = st;
				});
			addTextField(textField);
		}
	}
}

void PropertiesBox::addToggleField(const tgui::String &title) {
	auto group1 = tgui::Group::create({"100%", 24});

	auto label = tgui::Label::create(title);
	label->setSize({"50%", "100%"});
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	group1->add(label);

	auto value = tgui::ToggleButton::create();
	value->setSize({"50%", "100%"});
	value->setPosition("50%", 0);
	group1->add(value);

	layout->add(group1);
}

void PropertiesBox::addButton(const tgui::String &title,
							  std::function<void()> callback) {
	auto button = tgui::Button::create(title);
	button->setSize("100%", 24);
	button->onPress(callback);

	layout->add(button);
}

void PropertiesBox::addIntField(const tgui::String &title, int initialValue,
								std::function<void(float)> callback) {
	auto group = tgui::Group::create({"100%", 24});

	auto label = tgui::Label::create(title);
	label->setSize("50%", "100%");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	group->add(label);

	auto value = tgui::SpinControl::create(0.0f, 75.0f);
	value->setSize({"50%", "100%"});
	value->setPosition("50%", 0);
	value->setValue(initialValue);
	value->onValueChange(callback);
	group->add(value);

	layout->add(group);
}

void PropertiesBox::addIntField(IntField::Ptr field) {
	field->setSize({"100%", 24});
	layout->add(field);
}

void PropertiesBox::addFileField(FileField::Ptr field) {
	field->setSize({"100%", 24});
	layout->add(field);
}

void PropertiesBox::addTextField(TextField::Ptr field) {
	field->setSize({"100%", 24});
	layout->add(field);
}
