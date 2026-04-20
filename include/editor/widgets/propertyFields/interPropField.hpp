#ifndef _RPGPP_INTERPROPFIELD_H
#define _RPGPP_INTERPROPFIELD_H

#include <TGUI/SubwidgetContainer.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Label.hpp>

#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"

class InterPropField : public tgui::SubwidgetContainer {
private:
	void updateSize();

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::Button::Ptr value;
	tgui::Button::Ptr remove;

	typedef std::shared_ptr<InterPropField> Ptr;
	typedef std::shared_ptr<const InterPropField> ConstPtr;

	InterPropField(const char *typeName = "InterPropField", bool initRenderer = true);

	static InterPropField::Ptr create();
	static InterPropField::Ptr copy(InterPropField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
};

#endif