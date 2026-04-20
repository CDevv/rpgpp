#ifndef _RPGPP_LISTFIELD_H
#define _RPGPP_LISTFIELD_H

#include <memory>
#include <vector>

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"

template <typename T>
class ListField : public tgui::SubwidgetContainer {
private:
	std::vector<T> items;

	void updateSize() {
		label->setPosition({PADDING, 0});
		label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
		value->setPosition({getSize().x * 0.5, 0});
		value->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	}

public:
	tgui::Label::Ptr label;
	tgui::Button::Ptr value;

	typedef std::shared_ptr<ListField> Ptr;
	typedef std::shared_ptr<const ListField> ConstPtr;

	ListField(const char *typeName = "ListField", bool initRenderer = true)
		: tgui::SubwidgetContainer(typeName, initRenderer) {
		label = tgui::Label::create("Label");
		label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
		label->setVerticalAlignment(tgui::VerticalAlignment::Center);
		value = tgui::Button::create();

		m_container->add(label);
		m_container->add(value);

		updateSize();
	}

	static ListField::Ptr create() { return std::make_shared<ListField>(); }

	static ListField::Ptr copy(ListField::ConstPtr widget) {
		if (widget)
			return std::static_pointer_cast<ListField>(widget->clone());
		else
			return nullptr;
	}

	void setSize(const tgui::Layout2d &size) override {
		tgui::SubwidgetContainer::setSize(size);
		updateSize();
	}

protected:
	Widget::Ptr clone() const override { return std::make_shared<ListField>(*this); }
};

#endif