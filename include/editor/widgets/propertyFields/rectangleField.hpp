#ifndef _RPGPP_RECTANGLEFIELD_H
#define _RPGPP_RECTANGLEFIELD_H

#include "TGUI/Signal.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "raylib.h"
class RectangleField : public tgui::SubwidgetContainer {
  private:
	void updateSize();
	tgui::Label::Ptr l_x;
	tgui::Label::Ptr l_y;
	tgui::Label::Ptr l_width;
	tgui::Label::Ptr l_height;
	tgui::SpinControl::Ptr value_x;
	tgui::SpinControl::Ptr value_y;
	tgui::SpinControl::Ptr value_width;
	tgui::SpinControl::Ptr value_height;

  protected:
	tgui::Widget::Ptr clone() const override;

  public:
	tgui::Label::Ptr label;
	void setValue(Rectangle value);
	Rectangle getValue();
	tgui::SignalTyped<Rectangle> onChange = {"onChange"};

	typedef std::shared_ptr<RectangleField> Ptr;
	typedef std::shared_ptr<const RectangleField> ConstPtr;

	RectangleField(const char *typeName = "RectangleField",
				   bool initRenderer = true);

	static Ptr create();
	static Ptr copy(ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
};

#endif
