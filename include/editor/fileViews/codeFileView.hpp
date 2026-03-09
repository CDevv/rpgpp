
#ifndef RPGPP_CODEFILEVIEW_H
#define RPGPP_CODEFILEVIEW_H
#include "fileView.hpp"
#include "views/codeView.hpp"

class CodeFileView : public FileView {
	CodeView::Ptr codeEditor;

  public:
	CodeFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif // RPGPP_CODEFILEVIEW_H
