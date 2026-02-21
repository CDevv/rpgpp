
#include "widgets/codeEditor.hpp"
#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"
#include "TGUI/Backend/Renderer/BackendText.hpp"
#include "TGUI/Backend/Window/BackendGui.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Text.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "syntaxHighlighter.hpp"
#include "tree_sitter/tree-sitter-lua.h"
#include <TGUI/Event.hpp>
#include <TGUI/Font.hpp>
#include <TGUI/TextStyle.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <sys/types.h>
#include <tree_sitter/api.h>

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

using namespace tgui;

#define CODE_EDITOR_LEFT_COLUMN 100.0f

// TODO: IMPLEMENT A BETTER SYNTAX COLORS MAP!
std::map<std::string, EditorHighlighting::TextStyling> SYNTAX_COLORS = {
	{"ERROR", {tgui::Color::Red, tgui::TextStyle::Regular}},
	{"local", {tgui::Color::Red, tgui::TextStyle::Regular}},
	{"number", {tgui::Color::Green, tgui::TextStyle::Regular}},
	{"string_content", {tgui::Color::Cyan, tgui::TextStyle::Regular}},
	{"identifier", {tgui::Color::Magenta, tgui::TextStyle::Regular}},
	{"function", {tgui::Color::Yellow, tgui::TextStyle::Regular}},
	{"function_call", {tgui::Color::Yellow, tgui::TextStyle::Regular}},
	{"\"", {tgui::Color::Cyan, tgui::TextStyle::Regular}},
	{"(", {tgui::Color::White, tgui::TextStyle::Regular}},
	{")", {tgui::Color::White, tgui::TextStyle::Regular}},
	{"end", {tgui::Color::Yellow, tgui::TextStyle::Regular}},
	{"--", {tgui::Color(84, 81, 103), tgui::TextStyle::Regular}},
	{"comment_content", {tgui::Color(84, 81, 103), tgui::TextStyle::Regular}}};

CodeEditor::CodeEditor() {

	this->syntaxParser = ts_parser_new();

	ts_parser_set_language(this->syntaxParser, tree_sitter_lua());

	this->onTextChange.connect([&](const tgui::String &text) {
		// this->constructHighlightedText(text, this->highlightTree.size() > 0);
		textWhole = constructText(m_text);
	});

	this->setMouseCursor(Cursor::Type::Text);
}

CodeEditor::~CodeEditor() {
	ts_parser_delete(this->syntaxParser);
	if (tsTree != nullptr) {
		ts_tree_delete(tsTree);
	}
}

tgui::Vector2f CodeEditor::findCharacterPosWhole(std::size_t pos) const {
	return textWhole.findCharacterPos(pos);
}

CodeEditor::Ptr CodeEditor::create() { return std::make_shared<CodeEditor>(); }

void CodeEditor::parseNode(
	const TSTreeCursor &cursor, const TSNode &node,
	std::list<EditorHighlighting::TextPiece> &list) const {

	auto nodeStartPoint = ts_node_start_point(node);
	auto nodeEndPoint = ts_node_end_point(node);

	if (ts_node_child_count(node) == 0) {
		printf("%s. \n", ts_node_type(node));

		auto row = static_cast<int>(ts_node_start_point(node).row);
		auto column = static_cast<int>(ts_node_start_point(node).column);

		if (row > m_lines.size() - 1) {
			return;
		}

		auto rowString = m_lines[row].toStdString();
		std::cout << "PARSE ROW: " << row << std::endl;

		std::string lineBefore =
			TextSubtext(rowString.c_str(), 0, ts_node_start_point(node).column);

		auto textLineBefore = constructText(lineBefore);

		/*
		std::string subtext =
			TextSubtext(m_text.toStdString().c_str(), ts_node_start_byte(node),
						ts_node_end_byte(node));

		int j = 0;
		std::string s = "";
		for (auto c : subtext) {
			if (c == '\n') {
				if (j != subtext.size() - 1) {
					printf("%s \n", s.c_str());
					s = "";
				}
			} else {
				s.push_back(c);
			}
			j++;
		}
		printf("%s \n", s.c_str());
		*/

		auto text = this->constructText(TextSubtext(
			m_lines[row].toStdString().c_str(),
			ts_node_start_point(node).column, ts_node_end_point(node).column));

		// auto text = constructText(s);

		if (SYNTAX_COLORS.count(ts_node_type(node)) > 0) {
			text.setColor(SYNTAX_COLORS[ts_node_type(node)].color);
		} else {
			text.setColor(tgui::Color::White);
		}

		auto a = textLineBefore.getLineWidth();
		auto b = textLineBefore.getLineHeight() * row;

		EditorHighlighting::TextPiece piece;
		piece.text = text;
		piece.pos = {a, b};
		piece.nodeStart = ts_node_start_byte(node);
		piece.nodeEnd = ts_node_end_byte(node);

		list.push_back(piece);
	}

	auto copy = ts_tree_cursor_copy(&cursor);
	if (ts_tree_cursor_goto_first_child(&copy))
		parseNode(copy, ts_tree_cursor_current_node(&copy), list);

	auto copy2 = ts_tree_cursor_copy(&cursor);
	if (ts_tree_cursor_goto_next_sibling(&copy2))
		parseNode(copy2, ts_tree_cursor_current_node(&copy2), list);
}

std::vector<EditorHighlighting::HighlighterStruct>
CodeEditor::getStructsFromText(const tgui::String &text_ref) {
	std::vector<EditorHighlighting::HighlighterStruct> highlighter = {};

	const auto textStr = text_ref.toStdString();
	const auto constCharStr = textStr.c_str();
	TSTree *syntaxTree = ts_parser_parse_string(
		this->syntaxParser, nullptr, constCharStr, strlen(constCharStr));

	const auto rootNode = ts_tree_root_node(syntaxTree);
	const auto cursor = ts_tree_cursor_new(rootNode);

	tsTree = syntaxTree;

	return highlighter;
}

void CodeEditor::setCode(tgui::String text) {
	TextArea::setText(text);

	const auto textStr = getText().toStdString();
	const auto constCharStr = textStr.c_str();
	TSTree *syntaxTree = ts_parser_parse_string(
		this->syntaxParser, nullptr, constCharStr, strlen(constCharStr));

	tsTree = syntaxTree;

	const auto rootNode = ts_tree_root_node(tsTree);
	const auto cursor = ts_tree_cursor_new(rootNode);

	// Windows Line Endings Fix.
	text.replace("\r\n", "\n");
	m_lines = text.split(U"\n");

	parseNode(cursor, rootNode, list);
}

bool CodeEditor::leftMousePressed(tgui::Vector2f pos) {
	pos.x -= CODE_EDITOR_LEFT_COLUMN;
	return TextArea::leftMousePressed(pos);
}

void CodeEditor::textEntered(char32_t key) {
	if (m_readOnly)
		return;

	// Don't allow carriage return characters, they only cause trouble
	if (key == '\r')
		return;

	// Make sure we don't exceed our maximum characters limit
	if ((m_maxChars > 0) && (m_text.length() + 1 > m_maxChars))
		return;

	auto insert = TGUI_LAMBDA_CAPTURE_EQ_THIS() {
		deleteSelectedCharacters();

		const std::size_t caretPosition = getSelectionEnd();

		m_text.insert(caretPosition, 1, key);
		m_lines[m_selEnd.y].insert(m_selEnd.x, 1, key);

		TSInputEdit edit;
		edit.start_byte = caretPosition;
		edit.start_point = {static_cast<uint32_t>(m_selEnd.x),
							static_cast<uint32_t>(m_selEnd.y)};

		edit.old_end_byte = caretPosition;
		edit.old_end_point = {static_cast<uint32_t>(m_selEnd.x),
							  static_cast<uint32_t>(m_selEnd.y)};

		edit.new_end_byte = caretPosition + 1;
		edit.new_end_point = {
			static_cast<uint32_t>(getColumnAt(caretPosition + 1)),
			static_cast<uint32_t>(getLineAt(caretPosition + 1))};

		if (tsTree == nullptr) {
			const auto textStr = getText().toStdString();
			const auto constCharStr = textStr.c_str();
			TSTree *syntaxTree =
				ts_parser_parse_string(this->syntaxParser, nullptr,
									   constCharStr, strlen(constCharStr));

			tsTree = syntaxTree;
		}

		// ts_tree_edit(tsTree, &edit);

		const auto textStr = m_text.toStdString();
		const auto constCharStr = textStr.c_str();
		TSTree *syntaxTree = ts_parser_parse_string(
			this->syntaxParser, nullptr, constCharStr, strlen(constCharStr));

		tsTree = syntaxTree;

		const auto rootNode = ts_tree_root_node(tsTree);
		const auto cursor = ts_tree_cursor_new(rootNode);

		std::vector<EditorHighlighting::HighlighterStruct> highlighter = {};
		// parseNode(cursor, rootNode, highlighter);

		// this->fuck = highlighter;

		list.clear();
		parseNode(cursor, rootNode, list);

		int count = 0;
		for (int i = 0; i < getCaretLine(); i++) {
			count += m_lines[i].size();
		}
		/*
		constructHighlightedText(highlighter, count,
								 count +
									 m_lines[getLineAt(caretPosition)].size());
									 */

		// Increment the caret position, unless you type a newline at the start
		// of a line while that line only existed due to word wrapping
		if ((key != U'\n') || (m_selEnd.x > 0) || (m_selEnd.y == 0) ||
			m_lines[m_selEnd.y - 1].empty() ||
			(m_text[caretPosition - 1] == U'\n')) {
			m_selStart.x++;
			m_selEnd.x++;
		}

		// Do not emit onCaretPositionChanged signal yet, as it could be
		// invalid.
		rearrangeText(true, false);
	};

	// If there is a scrollbar then inserting can't go wrong
	if (m_verticalScrollbar->getPolicy() != Scrollbar::Policy::Never) {
		const auto oldSelEnd = m_selEnd;
		insert();
		if (oldSelEnd != m_selEnd)
			onCaretPositionChange.emit(this);
	} else // There is no scrollbar, the text may not fit
	{
		// Store the data so that it can be reverted
		const auto oldText = m_text;
		const auto oldSelStart = m_selStart;
		const auto oldSelEnd = m_selEnd;

		// Try to insert the character
		insert();

		// Undo the insert if the text does not fit
		if (m_lines.size() >
			static_cast<std::size_t>(getInnerSize().y / m_lineHeight)) {
			m_text = oldText;
			m_selStart = oldSelStart;
			m_selEnd = oldSelEnd;

			rearrangeText(true);
		}
		// If the caret position changed, emit signal.
		else if (oldSelEnd != m_selEnd) {
			onCaretPositionChange.emit(this);
		}
	}

	// The caret should be visible again
	m_caretVisible = true;
	m_animationTimeElapsed = {};

	onTextChange.emit(this, m_text);
}

void CodeEditor::keyPressed(const tgui::Event::KeyEvent &event) {
	if (event.code == Event::KeyboardKey::Backspace)
		backspaceKeyPressed();
	else
		tgui::TextArea::keyPressed(event);
}

void CodeEditor::backspaceKeyPressed() {
	if (m_readOnly)
		return;

	// Check that we did not select any characters
	if (m_selStart == m_selEnd) {
		const std::size_t pos = getSelectionEnd();
		if (pos > 0) {
			if (m_selEnd.x > 0) {
				// There is a specific case that we have to watch out for. When
				// we are removing the last character on a line which was placed
				// there by word wrap and a newline follows this character then
				// the caret has to be placed at the line above (before the
				// newline) instead of at the same line (after the newline)
				if ((m_lines[m_selEnd.y].length() == 1) && (pos > 1) &&
					(pos < m_text.length()) && (m_text[pos - 2] != '\n') &&
					(m_text[pos] == '\n') && (m_selEnd.y > 0)) {
					m_selEnd.y--;
					m_selEnd.x = m_lines[m_selEnd.y].length();
				} else // Just remove the character normally
					--m_selEnd.x;
			} else // At the beginning of the line
			{
				if (m_selEnd.y > 0) {
					--m_selEnd.y;
					m_selEnd.x = m_lines[m_selEnd.y].length();

					if ((m_text[pos - 1] != '\n') && m_selEnd.x > 0)
						--m_selEnd.x;
				}
			}

			m_selStart = m_selEnd;

			m_text.erase(pos - 1, 1);

			// treesitter

			TSInputEdit edit;
			edit.start_byte = pos;
			edit.start_point = {static_cast<uint32_t>(getColumnAt(pos)),
								static_cast<uint32_t>(getLineAt(pos))};

			edit.old_end_byte = pos;
			edit.old_end_point = {static_cast<uint32_t>(getColumnAt(pos)),
								  static_cast<uint32_t>(getLineAt(pos))};

			edit.new_end_byte = pos - 1;
			edit.new_end_point = {static_cast<uint32_t>(getColumnAt(pos - 1)),
								  static_cast<uint32_t>(getLineAt(pos - 1))};

			if (tsTree == nullptr) {
				const auto textStr = getText().toStdString();
				const auto constCharStr = textStr.c_str();
				TSTree *syntaxTree =
					ts_parser_parse_string(this->syntaxParser, nullptr,
										   constCharStr, strlen(constCharStr));

				tsTree = syntaxTree;
			}

			// ts_tree_edit(tsTree, &edit);

			const auto textStr = getText().toStdString();
			const auto constCharStr = textStr.c_str();
			TSTree *syntaxTree =
				ts_parser_parse_string(this->syntaxParser, nullptr,
									   constCharStr, strlen(constCharStr));

			tsTree = syntaxTree;

			const auto rootNode = ts_tree_root_node(tsTree);
			const auto cursor = ts_tree_cursor_new(rootNode);

			list.clear();
			parseNode(cursor, rootNode, list);

			// If the "special case" above passes, and we let rearrangeText()
			// emit the onCaretPositionChange signal, the same signal will be
			// emitted twice. So prevent sending signal in rearrangeText() and
			// always send it manually afterwards.
			rearrangeText(true, false);
			onCaretPositionChange.emit(this);
		}
	} else // When you did select some characters then delete them
		deleteSelectedCharacters();

	// The caret should be visible again
	m_caretVisible = true;
	m_animationTimeElapsed = {};

	onTextChange.emit(this, m_text);
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

				if (!m_textSelection2.getString().empty()) {
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

	const auto oldStates = states;

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
		target.drawFilledRect(states, {2, lineIndex.getLineHeight()},
							  tgui::Color::White);
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

			auto leftSelection =
				max(selectionRect.left, CODE_EDITOR_LEFT_COLUMN);
			states.transform.translate({leftSelection, selectionRect.top});
			target.drawFilledRect(
				states,
				{selectionRect.width,
				 selectionRect.height +
					 (std::max(m_fontCached.getFontHeight(m_textSizeCached),
							   m_lineHeight) -
					  m_lineHeight)},
				tgui::Color::applyOpacity(m_selectedTextBackgroundColorCached,
										  m_opacityCached));
			states.transform.translate({-leftSelection, -selectionRect.top});
		}

		// Draw the text
		if (m_text.empty())
			target.drawText(states, m_defaultText);
		else {

			if (m_selStart == m_selEnd)
				states.transform.translate({CODE_EDITOR_LEFT_COLUMN, 0});

			Vector2f offsetPos = {0, 0};

			if (m_selStart != m_selEnd) {
				states.transform.translate({CODE_EDITOR_LEFT_COLUMN, 0});
				states.transform.translate({m_paddingCached.getLeft(), 0});
			}

			for (auto &textPiece : list) {
				states.transform.translate(textPiece.pos);
				if ((textPiece.pos.y >= m_verticalScrollbar->getValue()) &&
					(textPiece.pos.y <
					 m_verticalScrollbar->getValue() + clipHeight)) {
					target.drawText(states, textPiece.text);
				}
				states.transform.translate(-textPiece.pos);
			}

			if (m_selStart != m_selEnd) {
				states.transform.translate({-CODE_EDITOR_LEFT_COLUMN, 0});
				states.transform.translate({-m_paddingCached.getLeft(), 0});
			}
			states.transform.translate(-offsetPos);

			if (m_selStart == m_selEnd)
				states.transform.translate({-CODE_EDITOR_LEFT_COLUMN, 0});
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

Text CodeEditor::constructText(const tgui::String &text,
							   const tgui::Color &color) const {
	auto cloneText = Text{};

	cloneText.setFont(m_fontCached);
	cloneText.setColor(color);
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
	if (const auto caret = a; caret == 0)
		return 1;
	else
		return m_text.substr(0, caret).count('\n') + 1;
}
