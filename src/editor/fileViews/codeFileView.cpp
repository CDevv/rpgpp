#include "fileViews/codeFileView.hpp"

#include "scriptFile.h"
#include "widgets/codeEditor.hpp"

CodeFileView::CodeFileView() {
	codeEditor = CodeEditor::create();
	codeEditor->setSize({"100%", "100%"});
	widgetContainer.push_back(codeEditor);
}

void CodeFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<ScriptFile> *>(variant);
		codeEditor->setText(ptr->get()->getFileContents());
		codeEditor->setTextSize(24);
		addWidgets(layout);
	}
}
