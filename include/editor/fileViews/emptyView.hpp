#ifndef _RPGPP_FILEVIEWS_EMPTYVIEW_H
#define _RPGPP_FILEVIEWS_EMPTYVIEW_H

#include "fileView.hpp"
#include "variant.hpp"

class EmptyFileView : public FileView {
  public:
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif