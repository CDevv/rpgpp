#ifndef _RPGPP_FILEVIEWS_EMPTYVIEW_H
#define _RPGPP_FILEVIEWS_EMPTYVIEW_H

#include "fileView.hpp"

class EmptyFileView : public FileView {
  public:
	void init(tgui::Group::Ptr layout) override;
};

#endif