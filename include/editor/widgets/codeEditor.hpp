
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
#include <string>
#include <vector>
class CodeEditor : public tgui::TextArea {
  public:
	static Ptr create();
	CodeEditor();
	~CodeEditor();

	void keyPressed(const tgui::Event::KeyEvent &event) override;
	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

  private:
	TSParser *syntaxParser;

	tgui::Text constructText(const tgui::String &,
							 tgui::Vector2f position) const;
	tgui::Text constructText(const tgui::String &text) const;
	tgui::Text constructText(const tgui::String &text,
							 const tgui::Color &color) const;
	bool canGainFocus() const override;
	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	bool scrolled(float delta, tgui::Vector2f pos, bool touch) override;

	std::vector<tgui::Text> highlightTree;
	void parseNode(const TSTreeCursor &cursor, const TSNode &node,
				   std::vector<EditorHighlighting::HighlighterStruct> &vector);
	void constructHighlightedText(const tgui::String &text);

	tgui::Vector2<std::size_t> findCaretPosition(tgui::Vector2f pos) const;
	void updateSelectionTexts();
	void recalculatePositions();
	void recalculateVisibleLines();
	std::size_t getColumnAt(std::size_t a) const;
	std::size_t getLineAt(std::size_t a) const;
};

#endif // RPGPP_CODEEDITOR_H
