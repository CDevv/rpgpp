#include "widgets/propertyFields/interPropField.hpp"

#include <TGUI/Widgets/EditBox.hpp>
#include <memory>

#include "TGUI/Texture.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

InterPropField::InterPropField(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	value = tgui::Button::create();
	remove = tgui::Button::create("Remove");

	m_container->add(label, "Label");
	m_container->add(value, "Value");
	m_container->add(remove, "Remove");

	updateSize();
}

InterPropField::Ptr InterPropField::create() { return std::make_shared<InterPropField>(); }

InterPropField::Ptr InterPropField::copy(InterPropField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<InterPropField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr InterPropField::clone() const { return std::make_shared<InterPropField>(*this); }

void InterPropField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void InterPropField::updateSize() {
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.33f - PADDING, getSize().y});
	value->setPosition({getSize().x * 0.33f, 0});
	value->setSize({getSize().x * 0.33f, getSize().y});
	remove->setPosition({getSize().x * 0.67f, 0});
	remove->setSize({getSize().x * 0.33f - PADDING, getSize().y});
}