#ifndef _RPGPP_FILEVIEW_H
#define _RPGPP_FILEVIEW_H

#include "TGUI/Widgets/Group.hpp"
#include "variant.hpp"
class FileView {
  protected:
	VariantWrapper *variant;

  public:
	FileView();
	virtual ~FileView();

	virtual void init(tgui::Group::Ptr layout, VariantWrapper *variant) {}
};

#endif