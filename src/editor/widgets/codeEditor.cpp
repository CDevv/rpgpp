
#include "codeEditor.hpp"
#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"
#include "TGUI/Backend/Renderer/BackendText.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Text.hpp"

#include <cmath>
#include <iostream>
#include <string>

using namespace tgui;

#define CODE_EDITOR_LEFT_COLUMN 50.0f


CodeEditor::Ptr CodeEditor::create() { return std::make_shared<CodeEditor>(); }

void CodeEditor::keyPressed(const Event::KeyEvent &event) {
	TextArea::keyPressed(event);
}



void CodeEditor::draw(BackendRenderTarget &target, RenderStates states) const {
	target.drawFilledRect(states, getInnerSize(), Color::Black);
	const auto& innerSize = getInnerSize();
	const auto lines = this->m_text.split("\n");

	for (int i = 0; i < lines.size(); i++) {
		auto &currentLine = lines[i];
		auto leftWidth = 0.0f;
		const bool isSelected = i == this->getCaretLine() - 1;

		if (isSelected) {
			target.drawFilledRect(states, {innerSize.x, m_textBeforeSelection.getLineHeight()}, Color{50, 50, 50, 255});
			const auto textCaretSubstr = currentLine.substr(std::min(currentLine.size() - (this->getCaretColumn() - 1), currentLine.size()));
			leftWidth = Text::getLineWidth(textCaretSubstr, m_fontCached, m_textSize);
		}

		Text lineIndx = this->constructText(std::to_string(i + 1));
		lineIndx.setPosition({CODE_EDITOR_LEFT_COLUMN / 2 - lineIndx.getLineWidth(), 0});
		target.drawText(states, lineIndx);


		Text text = this->constructText(currentLine);
		text.setPosition({CODE_EDITOR_LEFT_COLUMN, 0});

		target.drawText(states, text);
		auto leftOffset = leftWidth + CODE_EDITOR_LEFT_COLUMN;

		if (isSelected && m_focused && m_caretVisible && (m_caretWidthCached > 0)) {
			states.transform.translate({leftOffset, 0});
			target.drawFilledRect(states, {2, m_textBeforeSelection.getLineHeight()}, Color::White);
			states.transform.translate({-leftOffset, 0});
		}

		states.transform.translate({0, lineIndx.getLineHeight()});
	}
}

Text CodeEditor::constructText(const tgui::String & text,
							   const Vector2f position) const {
	auto cloneText = Text{};

	cloneText.setFont(m_fontCached);
	cloneText.setColor(Color::White);
	cloneText.setString(text);
	cloneText.setPosition(position);

	return cloneText;
}

Text CodeEditor::constructText(const tgui::String &text) const {
	auto cloneText = Text{};

	cloneText.setFont(m_fontCached);
	cloneText.setColor(Color::White);
	cloneText.setString(text);

	return cloneText;
}