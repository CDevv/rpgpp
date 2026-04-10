#ifndef _RPGPP_FILEVIEWS_IMAGEFILEVIEW_H
#define _RPGPP_FILEVIEWS_IMAGEFILEVIEW_H

#include "TGUI/Widgets/Label.hpp"
#include "fileViews/fileView.hpp"
#include "views/imageView.hpp"

class ImageFileView : public FileView {
public:
	ImageFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

private:
	ImageView::Ptr imageView;
	tgui::Label::Ptr infoLabel;
};

#endif