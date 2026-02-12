#include "codeFileView.hpp"

#include "codeEditor.hpp"
#include "scriptFile.h"

CodeFileView::CodeFileView() {
	codeEditor = CodeEditor::create();
	codeEditor->setSize({"100%", "100%"});
	widgetContainer.push_back(codeEditor);
}

void CodeFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<ScriptFile>*>(variant);
		codeEditor->setText(ptr->get()->getFileContents());
		addWidgets(layout);
	}
}
