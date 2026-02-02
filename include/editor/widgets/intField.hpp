#ifndef _RPGPP_INTFIELD_H
#define _RPGPP_INTFIELD_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include <memory>

class IntField : public tgui::SubwidgetContainer {
  private:
	void updateSize();

  protected:
	tgui::Widget::Ptr clone() const override;

  public:
	tgui::Label::Ptr label;
	tgui::SpinControl::Ptr value;

	typedef std::shared_ptr<IntField> Ptr;
	typedef std::shared_ptr<const IntField> ConstPtr;

	IntField(const char *typeName = "IntField", bool initRenderer = true);

	static IntField::Ptr create();
	static IntField::Ptr copy(IntField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
};

#endif