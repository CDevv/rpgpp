
#ifndef RPGPP_CODEEDITOR_H
#define RPGPP_CODEEDITOR_H
#include "TGUI/String.hpp"
#include "TGUI/Text.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include "syntaxHighlighter.hpp"
#include "tree_sitter/api.h"
#include <TGUI/Color.hpp>
#include <cstddef>
#include <list>
#include <memory>
#include <string>
#include <vector>
class CodeEditor : public tgui::TextArea {
  public:
	typedef std::shared_ptr<CodeEditor> Ptr;

	static Ptr create();
	CodeEditor();
	~CodeEditor();

	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

	void setCode(tgui::String text);

  private:
	TSParser *syntaxParser;
	TSTree *tsTree{nullptr};
	std::vector<std::vector<tgui::Text>> highlightTree;
	std::vector<EditorHighlighting::TextPiece> highlightVec;
	std::vector<EditorHighlighting::HighlighterStruct> fuck;
	tgui::Text textWhole;
	std::list<EditorHighlighting::TextPiece> list;

	tgui::Text constructText(const tgui::String &,
							 tgui::Vector2f position) const;
	tgui::Text constructText(const tgui::String &text) const;
	tgui::Text constructText(const tgui::String &text,
							 const tgui::Color &color) const;

	bool canGainFocus() const override;
	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	bool scrolled(float delta, tgui::Vector2f pos, bool touch) override;
	void textEntered(char32_t key) override;
	void keyPressed(const tgui::Event::KeyEvent &event) override;

	void backspaceKeyPressed();

	void parseNode(const TSTreeCursor &cursor, const TSNode &node,
				   std::list<EditorHighlighting::TextPiece> &list) const;
	void constructHighlightedText(
		std::vector<EditorHighlighting::HighlighterStruct> &highlight,
		bool editOnlyOnCaret = false);
	void constructHighlightedText(
		std::vector<EditorHighlighting::HighlighterStruct> &highlight,
		int start, int end);
	void changeHighlightedText(const tgui::String &text, int lineIdx);

	std::vector<EditorHighlighting::HighlighterStruct>
	getStructsFromText(const tgui::String &text);

	tgui::Vector2<std::size_t> findCaretPosition(tgui::Vector2f pos) const;

	void updateSelectionTexts();
	void recalculatePositions();

	void recalculateVisibleLines();
	std::size_t getColumnAt(std::size_t a) const;
	std::size_t getLineAt(std::size_t a) const;

	tgui::Vector2f findCharacterPosWhole(std::size_t pos) const;
};

#endif // RPGPP_CODEEDITOR_H
