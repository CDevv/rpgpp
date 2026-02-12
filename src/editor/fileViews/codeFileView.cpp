#include "codeFileView.h"

#include "codeEditor.hpp"

CodeFileView::CodeFileView() {
	// TODO: Add undo / redo feature.

	const auto codeTextArea = CodeEditor::create();
	codeTextArea->setSize({"100%", "100%"});
	widgetContainer.push_back(codeTextArea);
}

void CodeFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;
	addWidgets(layout);
}