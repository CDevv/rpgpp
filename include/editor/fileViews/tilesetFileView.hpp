#ifndef _RPGPP_FILEVIEWS_TILESETFILEVIEW_H
#define _RPGPP_FILEVIEWS_TILESETFILEVIEW_H

#include "fileView.hpp"
#include "tileSetView.hpp"
#include "variant.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include "worldView.hpp"

class TileSetFileView : public FileView {
  private:
	TileSetView::Ptr worldView;
	IntField::Ptr widthField;
	IntField::Ptr heightField;
	FileField::Ptr textureFile;

  public:
	TileSetFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif