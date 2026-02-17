#ifndef RPGPP_TOOLTIP_H
#define RPGPP_TOOLTIP_H

#include "TGUI/Widgets/Label.hpp"
class Tooltip : public tgui::Label {
public:
	Tooltip(const std::string& text);
	typedef std::shared_ptr<Tooltip> Ptr;
	typedef std::shared_ptr<const Tooltip> ConstPtr;

	static Ptr create(const std::string& text);
	static Ptr copy(ConstPtr widget);
protected:
	tgui::Widget::Ptr clone() const override;
};

#endif
