
#ifndef RPGPP_CODEFILEVIEW_H
#define RPGPP_CODEFILEVIEW_H
#include "fileView.hpp"

class CodeFileView : public FileView {
  public:
	CodeFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif // RPGPP_CODEFILEVIEW_H
