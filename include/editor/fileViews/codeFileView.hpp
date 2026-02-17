
#ifndef RPGPP_CODEFILEVIEW_H
#define RPGPP_CODEFILEVIEW_H
#include "widgets/codeEditor.hpp"
#include "fileView.hpp"

class CodeFileView : public FileView {
	CodeEditor::Ptr codeEditor;

	public:
		CodeFileView();
		void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif // RPGPP_CODEFILEVIEW_H
