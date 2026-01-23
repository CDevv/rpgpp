#ifndef _RPGPP_FILEVIEWS_TILESETFILEVIEW_H
#define _RPGPP_FILEVIEWS_TILESETFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"

class TileSetFileView : public FileView {
  public:
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif