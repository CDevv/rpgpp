#include "widgets/propertyFields/boolField.hpp"

#include "TGUI/Widgets/CheckBox.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"
BoolField::BoolField(const char *typeName, bool initRenderer) : tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	value = tgui::CheckBox::create();

	m_container->add(label);
	m_container->add(value);

	updateSize();
}

BoolField::Ptr BoolField::create() { return std::make_shared<BoolField>(); }

BoolField::Ptr BoolField::copy(BoolField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<BoolField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr BoolField::clone() const { return std::make_shared<BoolField>(*this); }

void BoolField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void BoolField::updateSize() {
	auto height = getSize().y;
	auto checkboxSize = height;
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x - PADDING * 3 - checkboxSize, height});
	value->setSize({checkboxSize, height});
	value->setPosition({getSize().x - PADDING - checkboxSize, 0});
}
