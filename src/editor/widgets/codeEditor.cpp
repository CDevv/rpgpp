
#include "codeEditor.hpp"
#include "TGUI/Backend/Renderer/BackendText.hpp"
#include "TGUI/Text.hpp"

#include <cmath>
#include <iostream>
#include <string>



using namespace tgui;



CodeEditor::Ptr CodeEditor::create() { return std::make_shared<CodeEditor>(); }

void CodeEditor::keyPressed(const Event::KeyEvent &event) {
	TextArea::keyPressed(event);
}

Text CodeEditor::createText(const tgui::String &text,
							const tgui::Vector2f &position) {
	auto cloneText = Text(m_textBeforeSelection);
	cloneText.setString(text);
	cloneText.setPosition(position);
	return cloneText;
}