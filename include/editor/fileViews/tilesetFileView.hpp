#ifndef _RPGPP_FILEVIEWS_TILESETFILEVIEW_H
#define _RPGPP_FILEVIEWS_TILESETFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"
#include "views/tileSetView.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"

class TileSetFileView : public FileView {
private:
	static const int RIGHT_PANEL_W = 300;
	TileSetView::Ptr worldView;
	IntField::Ptr widthField;
	IntField::Ptr heightField;
	FileField::Ptr textureFile;

public:
	TileSetFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif
