#include "widgets/dialogueEditor.hpp"

#include <memory>
#include <sstream>
#include <string>

#include "TGUI/String.hpp"

DialogueEditor::Ptr DialogueEditor::create() { return std::make_shared<DialogueEditor>(); }

DialogueEditor::DialogueEditor() {
	this->onSelectionChange.connect([&] {
		if (!this->isFocused()) return;

		this->formattingSelectionStart = this->getSelectionStart();
		this->formattingSelectionEnd = this->getSelectionEnd();
		this->selectedText = this->getSelectedText();
	});
}

bool DialogueEditor::isSelectedTextEmpty() { return this->selectedText.empty(); }

void DialogueEditor::addXmlTag(std::string tagName) {
	auto text = this->getText();

	if (this->selectedText.contains("<") || this->selectedText.contains(">") || this->isSelectedTextEmpty()) {
		return;
	}

	std::stringstream ss{};

	ss << text.substr(0, this->formattingSelectionStart);
	ss << "<" << tagName << ">" << this->selectedText << "</" << tagName << ">";
	ss << text.substr(this->formattingSelectionEnd);

	this->selectedText = "";

	this->setText(ss.str());
}