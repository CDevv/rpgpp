#ifndef _RPGPP_BOOLFIELD_H
#define _RPGPP_BOOLFIELD_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/Label.hpp"
class BoolField : public tgui::SubwidgetContainer {
private:
	void updateSize();

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::CheckBox::Ptr value;

	typedef std::shared_ptr<BoolField> Ptr;
	typedef std::shared_ptr<const BoolField> ConstPtr;

	BoolField(const char *typeName = "BoolField", bool initRenderer = true);

	static Ptr create();
	static Ptr copy(ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
};

#endif
