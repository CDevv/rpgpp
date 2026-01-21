#ifndef _RPGPP_FILEVIEW_H
#define _RPGPP_FILEVIEW_H

#include "TGUI/Widgets/Group.hpp"
class FileView {
  public:
	FileView();
	virtual ~FileView();

	virtual void init(tgui::Group::Ptr layout) {}
};

#endif