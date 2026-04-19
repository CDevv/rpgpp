#include "widgets/dialogueEditor.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

constexpr const char* spaceLiteral{" "};

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

bool DialogueEditor::isTextNonEditable(std::string& tagName) {
	return this->selectedText.contains(tagName) || this->selectedText.empty();
}


void DialogueEditor::setTextAndReset(std::string text) {
	this->selectedText = "";
	this->setText(text);
}

void DialogueEditor::fixSelectionRange() {
	// This fixes an interesting TGUI issue where the formattingSelectionStart can sometimes be the end of the selection.
	// Thanks TGUI for being an awesome GUI library.
	// .w. - thefirey33, 2026

	if (this->formattingSelectionStart > this->formattingSelectionEnd) {
		auto selectionEnd = this->formattingSelectionEnd;

		this->formattingSelectionEnd = this->formattingSelectionStart;
		this->formattingSelectionStart = selectionEnd;
	}
}

void DialogueEditor::addXmlTag(std::string tagName) {
	this->fixSelectionRange();

	if (this->isTextNonEditable(tagName))
		return;

	auto text = this->getText();

	std::stringstream ss{};

	ss << text.substr(0, this->formattingSelectionStart);
	ss << "<" << tagName << ">" << this->selectedText << "</" << tagName << ">";
	ss << text.substr(this->formattingSelectionEnd);

	this->setTextAndReset(ss.str());
}

void DialogueEditor::addXmlTagWithProperties(std::string tagName, std::vector<XMLTagProperties> properties) {
	this->fixSelectionRange();

	if (this->isTextNonEditable(tagName))
		return;

	auto text = this->getText();

	std::stringstream ss{};

	ss << text.substr(0, this->formattingSelectionStart);
	ss << "<" << tagName << spaceLiteral;

	for (int i = 0; i < properties.size(); i++) {
		auto& val = properties[i];

		/**
		 * Construct an XML tag property string in the format "name=\"value\"" with a space separator
		 * between properties, except for the last one.
		 *
		 * This is very confusing, i know.
		 */
		ss << val.propertyName << "=\"" << val.propertyValue << "\"" << (i < properties.size() - 1 ? spaceLiteral : std::string{});
	}

	ss << ">" << this->selectedText << "</" << tagName << ">";
	ss << text.substr(this->formattingSelectionEnd);

	this->setTextAndReset(ss.str());
}
