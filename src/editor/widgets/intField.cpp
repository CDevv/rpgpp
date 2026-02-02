#include "widgets/intField.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include <memory>

IntField::IntField(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	value = tgui::SpinControl::create(0, 100);

	m_container->add(label);
	m_container->add(value);

	updateSize();
}

IntField::Ptr IntField::create() { return std::make_shared<IntField>(); }

IntField::Ptr IntField::copy(IntField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<IntField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr IntField::clone() const {
	return std::make_shared<IntField>(*this);
}

void IntField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void IntField::updateSize() {
	label->setSize({getSize().x * 0.5f, getSize().y});
	value->setSize({getSize().x * 0.5f, getSize().y});

	value->setPosition({getSize().x * 0.5, 0});
}