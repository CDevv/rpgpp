#ifndef _RPGPP_DIALOGUEEDITOR_HPP
#define _RPGPP_DIALOGUEEDITOR_HPP

#include <cstddef>
#include <memory>
#include <string>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/TextArea.hpp"
class DialogueEditor : public tgui::TextArea {
private:
	std::size_t formattingSelectionStart;
	std::size_t formattingSelectionEnd;
	tgui::String selectedText;

public:
	typedef std::shared_ptr<DialogueEditor> Ptr;
	DialogueEditor();

	static DialogueEditor::Ptr create();

	void addXmlTag(std::string tagName);
	bool isSelectedTextEmpty();
};

#endif /* _RPGPP_DIALOGUEEDITOR_HPP */
