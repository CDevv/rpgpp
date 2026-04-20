
#ifndef _RPGPP_SELECTFIELD_H
#define _RPGPP_SELECTFIELD_H

#include <memory>

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"

class SelectField : public tgui::Group {
private:
	void updateSize();

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::ComboBox::Ptr value;

	typedef std::shared_ptr<SelectField> Ptr;
	typedef std::shared_ptr<const SelectField> ConstPtr;

	SelectField(const char *typeName = "SelectField", bool initRenderer = true);

	static SelectField::Ptr create();
	static SelectField::Ptr copy(SelectField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
};

#endif
