#ifndef _RPGPP_FILEVIEWS_TILESETFILEVIEW_H
#define _RPGPP_FILEVIEWS_TILESETFILEVIEW_H

#include "fileView.hpp"

class TileSetFileView : public FileView {
  public:
	void init(tgui::Group::Ptr layout) override;
};

#endif