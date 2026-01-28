#ifndef _RPGPP_FILEVIEW_H
#define _RPGPP_FILEVIEW_H

#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "variant.hpp"
#include <vector>
class FileView {
  protected:
	VariantWrapper *variant;
	std::vector<tgui::Widget::Ptr> widgetContainer;

  public:
	FileView();
	virtual ~FileView();

	virtual void init(tgui::Group::Ptr layout, VariantWrapper *variant) {}
	void addWidgets(tgui::Group::Ptr layout);
};

#endif