#include "widgets/propertyFields/intField.hpp"

#include <memory>

#include "TGUI/Texture.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

IntField::IntField(const char *typeName, bool initRenderer) : tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	value = tgui::SpinControl::create(0, 100);
	remove = tgui::BitmapButton::create();
	remove->setSize({0, 0});

	m_container->add(label);
	m_container->add(value);

	auto closeImagePath = Editor::instance->getFs().getResourcePath("close.png");
	tgui::Texture imageTexture(closeImagePath);
	remove->setImage(imageTexture);
	remove->setVisible(false);
	m_container->add(remove);

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

tgui::Widget::Ptr IntField::clone() const { return std::make_shared<IntField>(*this); }

void IntField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void IntField::updateSize() {
	if (!removable) {
		label->setPosition({PADDING, 0});
		label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
		value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
		value->setPosition({getSize().x * 0.5, 0});
		remove->setPosition({0, 0});
		remove->setSize({0, 0});
	} else {
		label->setPosition({PADDING, 0});
		label->setSize({getSize().x * 0.4f, getSize().y});
		value->setPosition({getSize().x * 0.4f, 0});
		value->setSize({getSize().x * 0.4f, getSize().y});
		remove->setPosition({getSize().x * 0.8f, 0});
		remove->setSize({getSize().x * 0.2f, getSize().y});
	}
}

void IntField::enableRemoving() {
	removable = true;
	remove->setVisible(true);
	updateSize();
}
