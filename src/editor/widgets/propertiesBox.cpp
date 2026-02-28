#include "widgets/propertiesBox.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/textField.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include "widgets/propertyFields/fieldConfig.hpp"

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
		if (item.value().is_number()) {
			auto intField = IntField::create();
			intField->label->setText(item.key());
			intField->value->setValue(item.value().get<float>());
			intField->value->onValueChange(
				[&j, item](float value) { j.at(item.key()) = value; });
			addIntField(intField);
		}
		if (item.value().is_boolean()) {
			auto boolField = BoolField::create();
			boolField->label->setText(item.key());
			boolField->value->setChecked(item.value().get<bool>());
			boolField->value->onChange(
				[&j, item](bool checked) { j.at(item.key()) = checked; });
			addBooleanField(boolField);
		}
		if (item.value().is_object()) {
			if (item.value().contains("propType")) {
				std::string propType = item.value().at("propType");

				auto fileField = FileField::create();
				fileField->label->setText(item.key());
				fileField->value->setText(
					item.value().at("value").get<std::string>());
				fileField->callback = [&j, item,
									   this](const tgui::String &filePath) {
					printf("%s \n", filePath.toStdString().c_str());
					printf("%s \n", GetFileNameWithoutExt(
										filePath.toStdString().c_str()));
					auto &ref = j.at(item.key());

					ref.at("value") =
						GetFileNameWithoutExt(filePath.toStdString().c_str());
				};

				if (propType == "dialogue") {
					fileField->pathFilters = {{"Dialogue", {"*.rdiag"}}};
				}

				addFileField(fileField);
			}
		}
	}
}

tgui::Button::Ptr PropertiesBox::constructButton(const tgui::String &title,
							  std::function<void()> callback) {
	auto button = tgui::Button::create(title);
	button->setSize(TextFormat("100%% - %d", PADDING * 2), 24);
	button->setPosition({PADDING, 0});
	button->onPress(callback);

	layout->add(button);

	return button;
}

void PropertiesBox::addButton(const tgui::String &title,
							  std::function<void()> callback) {
	this->constructButton(title, callback);
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

void PropertiesBox::addBooleanField(BoolField::Ptr field) {
	field->setSize({"100%", 24});
	layout->add(field);
}

void PropertiesBox::addRectangleField(RectangleField::Ptr field) {
	field->setSize({"100%", 48});
	layout->add(field);
}
