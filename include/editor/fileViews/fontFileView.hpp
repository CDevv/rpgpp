#ifndef _RPGPP_FILEVIEWS_FONTFILEVIEW_H
#define _RPGPP_FILEVIEWS_FONTFILEVIEW_H

#include "fileViews/fileView.hpp"
#include "views/fontView.hpp"

class FontFileView : public FileView {
  public:
	FontFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

  private:
	FontView::Ptr fontView;
};

#endif
