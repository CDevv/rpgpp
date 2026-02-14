
#include "widgets/codeEditor.hpp"
#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"
#include "TGUI/Backend/Renderer/BackendText.hpp"
#include "TGUI/Backend/Window/BackendGui.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Text.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include "raylib.h"
#include "tree_sitter/tree-sitter-lua.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <tree_sitter/api.h>

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

using namespace tgui;

#define CODE_EDITOR_LEFT_COLUMN 100.0f

CodeEditor::Ptr CodeEditor::create() { return std::make_shared<CodeEditor>(); }

void CodeEditor::parseNode(TSTreeCursor cursor, TSNode node) {
	printf("%s %u %u \n", ts_node_type(node), ts_node_start_byte(node),
		   ts_node_end_byte(node));

	
	auto copy = ts_tree_cursor_copy(&cursor);
	bool hasChild = ts_tree_cursor_goto_first_child(&copy);
	if (hasChild)
		parseNode(copy, ts_tree_cursor_current_node(&copy));

	auto copy2 = ts_tree_cursor_copy(&cursor);
	bool hasSibling = ts_tree_cursor_goto_next_sibling(&copy2);
	if (hasSibling)
		parseNode(copy2, ts_tree_cursor_current_node(&copy2));
}

void CodeEditor::parseSyntaxTree(const tgui::String &text) {
	this->previewText.clear();

	auto textStr = text.toStdString();
	auto constCharStr = textStr.c_str();
	TSTree *syntaxTree = ts_parser_parse_string(
		this->syntaxParser, nullptr, constCharStr, strlen(constCharStr));

	auto rootNode = ts_tree_root_node(syntaxTree);
	auto cursor = ts_tree_cursor_new(rootNode);

	this->parseNode(cursor, rootNode);

	ts_tree_delete(syntaxTree);
}

void CodeEditor::keyPressed(const Event::KeyEvent &event) {
	// FIXME: ADD TAB KEY FUNCTIONALITY
	TextArea::keyPressed(event);
}

CodeEditor::CodeEditor() {
	this->syntaxParser = ts_parser_new();
	ts_parser_set_language(this->syntaxParser, tree_sitter_lua());

	this->onTextChange.connect(
		[&](const tgui::String &text) { this->parseSyntaxTree(text); });
}

CodeEditor::~CodeEditor() { ts_parser_delete(this->syntaxParser); }

bool CodeEditor::leftMousePressed(tgui::Vector2f pos) {
	pos.x -= CODE_EDITOR_LEFT_COLUMN;
	return TextArea::leftMousePressed(pos);
}

void CodeEditor::recalculatePositions() {
	if (!m_fontCached)
		return;

	float textOffset =
		Text::getExtraHorizontalPadding(m_fontCached, m_textSizeCached);

	// Calculate the position of the text objects
	m_selectionRects.clear();
	m_textBeforeSelection.setPosition(
		{textOffset + CODE_EDITOR_LEFT_COLUMN, 0});
	m_defaultText.setPosition({textOffset + CODE_EDITOR_LEFT_COLUMN, 0});

	if (m_selStart != m_selEnd) {
		auto selectionStart = m_selStart;
		auto selectionEnd = m_selEnd;

		if ((m_selStart.y > m_selEnd.y) ||
			((m_selStart.y == m_selEnd.y) && (m_selStart.x > m_selEnd.x)))
			std::swap(selectionStart, selectionEnd);

		float kerningSelectionStart = 0;
		if ((selectionStart.x > 0) &&
			(selectionStart.x < m_lines[selectionStart.y].length()))
			kerningSelectionStart = m_fontCached.getKerning(
				m_lines[selectionStart.y][selectionStart.x - 1],
				m_lines[selectionStart.y][selectionStart.x], m_textSizeCached,
				false);

		float kerningSelectionEnd = 0;
		if ((selectionEnd.x > 0) &&
			(selectionEnd.x < m_lines[selectionEnd.y].length()))
			kerningSelectionEnd = m_fontCached.getKerning(
				m_lines[selectionEnd.y][selectionEnd.x - 1],
				m_lines[selectionEnd.y][selectionEnd.x], m_textSizeCached,
				false);

		if (selectionStart.x > 0) {
			m_textSelection1.setPosition(
				{textOffset +
					 m_textBeforeSelection
						 .findCharacterPos(
							 m_textBeforeSelection.getString().length())
						 .x +
					 kerningSelectionStart + CODE_EDITOR_LEFT_COLUMN,
				 m_textBeforeSelection.getPosition().y +
					 (selectionStart.y * m_lineHeight)});
		} else
			m_textSelection1.setPosition(
				{textOffset + CODE_EDITOR_LEFT_COLUMN,
				 m_textBeforeSelection.getPosition().y +
					 (selectionStart.y * m_lineHeight)});

		m_textSelection2.setPosition(
			{textOffset + CODE_EDITOR_LEFT_COLUMN,
			 static_cast<float>((selectionStart.y + 1) * m_lineHeight)});

		if (!m_textSelection2.getString().empty() || (selectionEnd.x == 0)) {
			m_textAfterSelection1.setPosition(
				{textOffset +
					 m_textSelection2
						 .findCharacterPos(
							 m_textSelection2.getString().length())
						 .x +
					 kerningSelectionEnd + CODE_EDITOR_LEFT_COLUMN,
				 m_textSelection2.getPosition().y +
					 ((selectionEnd.y - selectionStart.y - 1) * m_lineHeight)});
		} else
			m_textAfterSelection1.setPosition(
				{m_textSelection1.getPosition().x +
					 m_textSelection1
						 .findCharacterPos(
							 m_textSelection1.getString().length())
						 .x +
					 kerningSelectionEnd,
				 m_textSelection1.getPosition().y});

		m_textAfterSelection2.setPosition(
			{textOffset + CODE_EDITOR_LEFT_COLUMN,
			 static_cast<float>(selectionEnd.y + 1) * m_lineHeight});

		// Recalculate the selection rectangles
		{
			m_selectionRects.emplace_back(m_textSelection1.getPosition().x,
										  static_cast<float>(selectionStart.y) *
											  m_lineHeight,
										  0.f, m_lineHeight);

			if (!m_lines[selectionStart.y].empty()) {
				m_selectionRects.back().width =
					m_textSelection1
						.findCharacterPos(m_textSelection1.getString().length())
						.x;

				// There is kerning when the selection is on just this line
				if (selectionStart.y == selectionEnd.y)
					m_selectionRects.back().width += kerningSelectionEnd;
			}

			/// TODO: Implement a way to calculate text size without creating a
			/// text object?
			Text tempText;
			tempText.setFont(m_fontCached);
			tempText.setCharacterSize(getTextSize());
			for (std::size_t i = selectionStart.y + 1; i < selectionEnd.y;
				 ++i) {
				m_selectionRects.back().width += textOffset;
				m_selectionRects.emplace_back(
					m_textSelection2.getPosition().x - textOffset,
					static_cast<float>(i) * m_lineHeight, textOffset,
					m_lineHeight);

				if (!m_lines[i].empty()) {
					tempText.setString(m_lines[i]);
					m_selectionRects.back().width +=
						tempText.findCharacterPos(tempText.getString().length())
							.x;
				}
			}

			if (selectionStart.y != selectionEnd.y) {
				m_selectionRects.back().width += textOffset;

				if (m_textSelection2.getString() != U"") {
					tempText.setString(
						m_lines[selectionEnd.y].substr(0, selectionEnd.x));
					m_selectionRects.emplace_back(
						m_textSelection2.getPosition().x - textOffset,
						static_cast<float>(selectionEnd.y) * m_lineHeight,
						textOffset +
							tempText
								.findCharacterPos(tempText.getString().length())
								.x +
							kerningSelectionEnd,
						m_lineHeight);
				} else
					m_selectionRects.emplace_back(
						0.f, static_cast<float>(selectionEnd.y) * m_lineHeight,
						textOffset, m_lineHeight);
			}
		}
	}

	if (m_parentGui) {
		const Vector2f caretPosition = {
			m_caretPosition.x + m_bordersCached.getLeft() +
				m_paddingCached.getLeft() -
				static_cast<float>(m_horizontalScrollbar->getValue()),
			m_caretPosition.y + m_bordersCached.getTop() +
				m_paddingCached.getTop() -
				static_cast<float>(m_verticalScrollbar->getValue())};
		const auto absoluteLineTopPos =
			getAbsolutePosition({0, caretPosition.y});
		const float caretHeight = std::max(
			m_fontCached.getFontHeight(m_textSizeCached), m_lineHeight);
		const FloatRect inputRect = {
			absoluteLineTopPos,
			getAbsolutePosition({getSize().x, caretPosition.y + caretHeight}) -
				absoluteLineTopPos};
		m_parentGui->updateTextCursorPosition(
			inputRect,
			getAbsolutePosition(
				{caretPosition.x + m_caretWidthCached, caretPosition.y}));
	}

	recalculateVisibleLines();
}

void CodeEditor::updateSelectionTexts() {
	// If there is no selection then just put the whole text in
	// m_textBeforeSelection
	if (m_selStart == m_selEnd) {
		String displayedText;
		for (const auto &line : m_lines)
			displayedText += line + U"\n";

		m_textBeforeSelection.setString(displayedText);
		m_textSelection1.setString(U"");
		m_textSelection2.setString(U"");
		m_textAfterSelection1.setString(U"");
		m_textAfterSelection2.setString(U"");
	} else // Some text is selected
	{
		auto selectionStart = m_selStart;
		auto selectionEnd = m_selEnd;

		if ((m_selStart.y > m_selEnd.y) ||
			((m_selStart.y == m_selEnd.y) && (m_selStart.x > m_selEnd.x)))
			std::swap(selectionStart, selectionEnd);

		// Set the text before the selection
		if (selectionStart.y > 0) {
			String string;
			for (std::size_t i = 0; i < selectionStart.y; ++i)
				string += m_lines[i] + U"\n";

			string += m_lines[selectionStart.y].substr(0, selectionStart.x);
			m_textBeforeSelection.setString(string);
		} else
			m_textBeforeSelection.setString(
				m_lines[0].substr(0, selectionStart.x));

		// Set the selected text
		if (m_selStart.y == m_selEnd.y) {
			m_textSelection1.setString(m_lines[selectionStart.y].substr(
				selectionStart.x, selectionEnd.x - selectionStart.x));
			m_textSelection2.setString(U"");
		} else {
			m_textSelection1.setString(m_lines[selectionStart.y].substr(
				selectionStart.x,
				m_lines[selectionStart.y].length() - selectionStart.x));

			String string;
			for (std::size_t i = selectionStart.y + 1; i < selectionEnd.y; ++i)
				string += m_lines[i] + U"\n";

			string += m_lines[selectionEnd.y].substr(0, selectionEnd.x);

			m_textSelection2.setString(string);
		}

		// Set the text after the selection
		{
			m_textAfterSelection1.setString(m_lines[selectionEnd.y].substr(
				selectionEnd.x,
				m_lines[selectionEnd.y].length() - selectionEnd.x));

			String string;
			for (std::size_t i = selectionEnd.y + 1; i < m_lines.size(); ++i)
				string += m_lines[i] + U"\n";

			m_textAfterSelection2.setString(string);
		}
	}

	// Check if the caret is located above or below the view
	if (m_verticalScrollbar->getPolicy() != Scrollbar::Policy::Never) {
		if (m_selEnd.y <= m_topLine)
			m_verticalScrollbar->setValue(
				static_cast<unsigned int>(m_selEnd.y * m_lineHeight));
		else if (m_selEnd.y + 1 >= m_topLine + m_visibleLines)
			m_verticalScrollbar->setValue(static_cast<unsigned int>(
				(m_selEnd.y * m_lineHeight) +
				std::max(m_fontCached.getFontHeight(m_textSizeCached),
						 m_lineHeight) +
				Text::getExtraVerticalPadding(m_textSizeCached) -
				m_verticalScrollbar->getViewportSize()));
	}

	// Position the caret
	{
		/// TODO: Implement a way to calculate text size without creating a text
		/// object?
		const float textOffset =
			Text::getExtraHorizontalPadding(m_fontCached, m_textSizeCached);
		Text tempText;
		tempText.setFont(m_fontCached);
		tempText.setCharacterSize(getTextSize());
		tempText.setString(m_lines[m_selEnd.y].substr(0, m_selEnd.x));

		float kerning = 0;
		if ((m_selEnd.x > 0) && (m_selEnd.x < m_lines[m_selEnd.y].length()))
			kerning = m_fontCached.getKerning(
				m_lines[m_selEnd.y][m_selEnd.x - 1],
				m_lines[m_selEnd.y][m_selEnd.x], m_textSizeCached, false);

		const float caretLeft =
			textOffset + CODE_EDITOR_LEFT_COLUMN +
			tempText.findCharacterPos(tempText.getString().length()).x +
			kerning;
		const float caretTop = static_cast<float>(m_selEnd.y) * m_lineHeight;
		m_caretPosition = {caretLeft, caretTop};
	}

	if (m_horizontalScrollbar->getPolicy() != Scrollbar::Policy::Never) {
		const unsigned int left = m_horizontalScrollbar->getValue();
		if (m_caretPosition.x <= left) {
			unsigned int newValue = static_cast<unsigned int>(std::max(
				0, static_cast<int>(m_caretPosition.x -
									(Text::getExtraHorizontalPadding(
										 m_fontCached, m_textSizeCached) *
									 2))));
			m_horizontalScrollbar->setValue(newValue);
		} else if (m_caretPosition.x >
				   (left + m_horizontalScrollbar->getViewportSize())) {
			unsigned int newValue = static_cast<unsigned int>(
				m_caretPosition.x +
				(Text::getExtraHorizontalPadding(m_fontCached,
												 m_textSizeCached) *
				 2) -
				m_horizontalScrollbar->getViewportSize());
			m_horizontalScrollbar->setValue(newValue);
		}
	}

	recalculatePositions();

	// Send an event when the selection changed
	if ((m_selStart != m_lastSelection.first) ||
		(m_selEnd != m_lastSelection.second)) {
		// Only send the event when there is an actual change, not when the
		// caret position moved
		if ((m_selStart != m_selEnd) ||
			(m_lastSelection.first != m_lastSelection.second))
			onSelectionChange.emit(this);

		m_lastSelection.first = m_selStart;
		m_lastSelection.second = m_selEnd;
	}
}

void CodeEditor::mouseMoved(tgui::Vector2f pos) {
	pos -= getPosition();

	if (!m_mouseHover)
		mouseEnteredWidget();

	// The mouse has moved so a double click is no longer possible
	m_possibleDoubleClick = false;

	// Check if the mouse event should go to the vertical scrollbar
	if (m_verticalScrollbar->isShown() &&
		(m_verticalScrollbar->isMouseDown() ||
		 m_verticalScrollbar->isMouseOnWidget(pos))) {
		m_verticalScrollbar->mouseMoved(pos);
		recalculateVisibleLines();
	}

	// Check if the mouse event should go to the horizontal scrollbar
	else if (m_horizontalScrollbar->isShown() &&
			 (m_horizontalScrollbar->isMouseDown() ||
			  m_horizontalScrollbar->isMouseOnWidget(pos))) {
		m_horizontalScrollbar->mouseMoved(pos);
	}

	// If the mouse is held down then you are selecting text
	else if (m_mouseDown) {
		auto caretPosition = findCaretPosition(pos);
		const auto oldSelEnd = m_selEnd;

		if (caretPosition != m_selEnd) {
			m_selEnd = caretPosition;
			updateSelectionTexts();
		}

		// Check if the caret is located above or below the view
		if (m_verticalScrollbar->getViewportSize() <
			m_verticalScrollbar->getMaximum()) {
			if (m_selEnd.y <= m_topLine)
				m_verticalScrollbar->setValue(
					static_cast<unsigned int>(m_selEnd.y * m_lineHeight));
			else if (m_selEnd.y + 1 >= m_topLine + m_visibleLines)
				m_verticalScrollbar->setValue(static_cast<unsigned int>(
					((m_selEnd.y + 1) * m_lineHeight) -
					m_verticalScrollbar->getViewportSize()));

			recalculateVisibleLines();
		}

		if (oldSelEnd != m_selEnd)
			onCaretPositionChange.emit(this);
	}

	// Inform the scrollbars that the mouse is not on them
	else {
		m_verticalScrollbar->mouseNoLongerOnWidget();
		m_horizontalScrollbar->mouseNoLongerOnWidget();
	}
}

tgui::Vector2<std::size_t>
CodeEditor::findCaretPosition(tgui::Vector2f position) const {
	position.x -= m_bordersCached.getLeft() + m_paddingCached.getLeft();
	position.y -= m_bordersCached.getTop() + m_paddingCached.getTop();

	position.x -= CODE_EDITOR_LEFT_COLUMN;

	// Don't continue when line height is 0 or when there is no font yet
	if ((m_lineHeight == 0) || (m_fontCached == nullptr))
		return {m_lines[m_lines.size() - 1].size(), m_lines.size() - 1};

	// Find on which line the mouse is
	std::size_t lineNumber;
	if (m_verticalScrollbar->getViewportSize() <
		m_verticalScrollbar->getMaximum()) {
		if (position.y + m_verticalScrollbar->getValue() < 0)
			return {0, 0};

		lineNumber = static_cast<std::size_t>(std::floor(
			(position.y + m_verticalScrollbar->getValue()) / m_lineHeight));
	} else {
		if (position.y < 0)
			return {0, 0};

		lineNumber =
			static_cast<std::size_t>(std::floor(position.y / m_lineHeight));
	}

	// Check if you clicked behind everything
	if (lineNumber + 1 > m_lines.size())
		return {m_lines[m_lines.size() - 1].size(), m_lines.size() - 1};

	// Find between which character the mouse is standing
	float width =
		Text::getExtraHorizontalPadding(m_fontCached, m_textSizeCached) -
		m_horizontalScrollbar->getValue();
	char32_t prevChar = 0;
	for (std::size_t i = 0; i < m_lines[lineNumber].size(); ++i) {
		float charWidth;
		const char32_t curChar = m_lines[lineNumber][i];
		// if (curChar == U'\n')
		//     return Vector2<std::size_t>(m_lines[lineNumber].getSize() - 1,
		//     lineNumber); // TextArea strips newlines but this code is kept
		//     for when this function is generalized
		// else
		if (curChar == U'\t')
			charWidth =
				static_cast<float>(
					m_fontCached.getGlyph(' ', getTextSize(), false).advance) *
				4;
		else
			charWidth = static_cast<float>(
				m_fontCached.getGlyph(curChar, getTextSize(), false).advance);

		const float kerning =
			m_fontCached.getKerning(prevChar, curChar, getTextSize(), false);
		if (width + charWidth + kerning <= position.x)
			width += charWidth + kerning;
		else {
			if (position.x < width + kerning + (charWidth / 2.0f))
				return {i, lineNumber};
			else
				return {i + 1, lineNumber};
		}

		prevChar = curChar;
	}

	// You clicked behind the last character
	return {m_lines[lineNumber].length(), lineNumber};
}

bool CodeEditor::scrolled(float delta, tgui::Vector2f pos, bool touch) {
	const bool horizontalScrollbarCanMove =
		(m_horizontalScrollbar->getViewportSize() <
		 m_horizontalScrollbar->getMaximum());
	const bool verticalScrollbarCanMove =
		(m_verticalScrollbar->getViewportSize() <
		 m_verticalScrollbar->getMaximum());

	bool scrollbarMoved = false;
	if (horizontalScrollbarCanMove && !touch &&
		(!verticalScrollbarCanMove ||
		 m_horizontalScrollbar->isMouseOnWidget(pos - getPosition()) ||
		 IsKeyReleased(KEY_LEFT_SHIFT))) {
		scrollbarMoved =
			m_horizontalScrollbar->scrolled(delta, pos - getPosition(), touch);
	} else if (verticalScrollbarCanMove) {
		scrollbarMoved =
			m_verticalScrollbar->scrolled(delta, pos - getPosition(), touch);
	}

	if (scrollbarMoved)
		recalculateVisibleLines();

	return scrollbarMoved;
}

void CodeEditor::recalculateVisibleLines() {
	if (m_lineHeight == 0)
		return;

	const float horiScrollOffset = m_horizontalScrollbar->isShown()
									   ? m_horizontalScrollbar->getSize().y
									   : 0.f;
	m_visibleLines =
		std::min(static_cast<std::size_t>((getInnerSize().y -
										   m_paddingCached.getTopPlusBottom() -
										   horiScrollOffset) /
										  m_lineHeight),
				 m_lines.size());

	// Store which area is visible
	if (m_verticalScrollbar->getViewportSize() <
		m_verticalScrollbar->getMaximum()) {
		m_topLine = static_cast<std::size_t>(m_verticalScrollbar->getValue() /
											 m_lineHeight);

		// The scrollbar may be standing between lines in which case one more
		// line is visible
		if (((static_cast<unsigned int>(getInnerSize().y -
										m_paddingCached.getTopPlusBottom() -
										horiScrollOffset) %
			  static_cast<unsigned int>(m_lineHeight)) != 0) ||
			((m_verticalScrollbar->getValue() %
			  static_cast<unsigned int>(m_lineHeight)) != 0))
			m_visibleLines++;
	} else // There is no vertical scrollbar
	{
		m_topLine = 0;
		m_visibleLines = std::min(
			static_cast<std::size_t>((getInnerSize().y -
									  m_paddingCached.getTopPlusBottom() -
									  horiScrollOffset) /
									 m_lineHeight),
			m_lines.size());
	}

	if (m_horizontalScrollbar->isShown())
		m_horizontalScrollbar->setPosition(
			m_bordersCached.getLeft(), getSize().y -
										   m_bordersCached.getBottom() -
										   m_horizontalScrollbar->getSize().y);

	if (m_verticalScrollbar->isShown())
		m_verticalScrollbar->setPosition({getSize().x -
											  m_bordersCached.getRight() -
											  m_verticalScrollbar->getSize().x,
										  m_bordersCached.getTop()});
}

void CodeEditor::draw(BackendRenderTarget &target, RenderStates states) const {
	const RenderStates statesForScrollbar = states;

	target.drawFilledRect(states, getInnerSize(), tgui::Color::Black);
	const auto &innerSize = getInnerSize();
	const auto lines = this->m_text.split("\n");

	auto oldStates = states;

	states.transform.translate(
		{m_paddingCached.getLeft(), m_paddingCached.getTop()});
	states.transform.translate(
		{-static_cast<float>(m_horizontalScrollbar->getValue()),
		 -static_cast<float>(m_verticalScrollbar->getValue())});
	for (int i = 0; i < lines.size(); i++) {
		// Text lineIndex = this->constructText(std::to_string(i + 1));
		Text lineIndex;
		lineIndex.setFont(m_textAfterSelection1.getFont());
		lineIndex.setCharacterSize(m_textSize);
		lineIndex.setString(std::to_string(i + 1));
		lineIndex.setColor(tgui::Color::White);
		lineIndex.setPosition(
			{CODE_EDITOR_LEFT_COLUMN - lineIndex.getLineWidth(), 0});
		target.drawText(states, lineIndex);

		states.transform.translate({0, lineIndex.getLineHeight()});
	}

	states = oldStates;

	{
		states.transform.translate(
			{m_paddingCached.getLeft(), m_paddingCached.getTop()});

		float clipWidth = getInnerSize().x - m_paddingCached.getLeftPlusRight();
		if (m_verticalScrollbar->isShown())
			clipWidth -= m_verticalScrollbar->getSize().x;

		float clipHeight =
			getInnerSize().y - m_paddingCached.getTopPlusBottom();
		if (m_horizontalScrollbar->isShown())
			clipHeight -= m_horizontalScrollbar->getSize().y;

		target.addClippingLayer(states, {{}, {clipWidth, clipHeight}});

		// Move the text according to the scrollars
		states.transform.translate(
			{-static_cast<float>(m_horizontalScrollbar->getValue()),
			 -static_cast<float>(m_verticalScrollbar->getValue())});

		// Draw the background of the selected text
		for (const auto &selectionRect : m_selectionRects) {
			states.transform.translate({selectionRect.left, selectionRect.top});
			target.drawFilledRect(
				states,
				{selectionRect.width,
				 selectionRect.height +
					 (std::max(m_fontCached.getFontHeight(m_textSizeCached),
							   m_lineHeight) -
					  m_lineHeight)},
				tgui::Color::applyOpacity(m_selectedTextBackgroundColorCached,
										  m_opacityCached));
			states.transform.translate(
				{-selectionRect.left, -selectionRect.top});
		}

		// Draw the text
		if (m_text.empty())
			target.drawText(states, m_defaultText);
		else {
			if (m_selStart == m_selEnd)
				states.transform.translate({CODE_EDITOR_LEFT_COLUMN, 0});

			target.drawText(states, m_textBeforeSelection);

			if (m_selStart == m_selEnd)
				states.transform.translate({-CODE_EDITOR_LEFT_COLUMN, 0});

			if (m_selStart != m_selEnd) {
				target.drawText(states, m_textSelection1);
				target.drawText(states, m_textSelection2);
				target.drawText(states, m_textAfterSelection1);
				target.drawText(states, m_textAfterSelection2);
			}
		}

		// Only draw the caret when needed
		if (m_focused && m_caretVisible && (m_caretWidthCached > 0)) {
			const float caretHeight = std::max(
				m_fontCached.getFontHeight(m_textSizeCached), m_lineHeight);
			states.transform.translate(
				{std::ceil(m_caretPosition.x - (m_caretWidthCached / 2.f)),
				 m_caretPosition.y});
			if (m_selStart == m_selEnd) {
				states.transform.translate({CODE_EDITOR_LEFT_COLUMN, 0});
			}
			target.drawFilledRect(
				states, {m_caretWidthCached, caretHeight},
				tgui::Color::applyOpacity(m_caretColorCached, m_opacityCached));
			if (m_selStart == m_selEnd) {
				states.transform.translate({-CODE_EDITOR_LEFT_COLUMN, 0});
			}
		}

		target.removeClippingLayer();
	}

	// Draw the scrollbars if needed
	if (m_verticalScrollbar->isShown())
		m_verticalScrollbar->draw(target, statesForScrollbar);

	if (m_horizontalScrollbar->isShown())
		m_horizontalScrollbar->draw(target, statesForScrollbar);
}

Text CodeEditor::constructText(const tgui::String &text,
							   const Vector2f position) const {
	auto cloneText = Text{};

	cloneText.setFont(m_fontCached);
	cloneText.setColor(tgui::Color::White);
	cloneText.setString(text);
	cloneText.setPosition(position);
	cloneText.setCharacterSize(m_textSize);

	return cloneText;
}

Text CodeEditor::constructText(const tgui::String &text) const {
	auto cloneText = Text{};

	cloneText.setFont(m_fontCached);
	cloneText.setColor(tgui::Color::White);
	cloneText.setString(text);
	cloneText.setCharacterSize(m_textSize);

	return cloneText;
}

bool CodeEditor::canGainFocus() const { return true; }

std::size_t CodeEditor::getColumnAt(std::size_t a) const {
	const auto caret = a;
	if (caret == 0)
		return 1;
	auto lineStart = m_text.rfind('\n', caret - 1);
	if (lineStart == String::npos)
		return caret + 1;
	else
		return caret - lineStart;
}

std::size_t CodeEditor::getLineAt(std::size_t a) const {
	const auto caret = a;
	if (caret == 0)
		return 1;
	else
		return m_text.substr(0, caret).count('\n') + 1;
}
