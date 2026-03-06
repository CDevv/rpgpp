#include "views/codeView.hpp"
#include "TGUI/Vector2.hpp"
#include "raylib.h"
#include "scriptFile.h"
#include "syntaxHighlighter.hpp"
#include "tree_sitter/api.h"
#include "tree_sitter/tree-sitter-lua.h"
#include "views/worldView.hpp"
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <utility>

CodeView::CodeView(ScriptFile *scriptFile) : scriptFile(scriptFile) {}

CodeView::CodeView() {}

void CodeView::doTree(TSTreeCursor cursor, TSNode node) {

	auto copy = ts_tree_cursor_copy(&cursor);
	bool hasChild = ts_tree_cursor_goto_first_child(&copy);
	if (hasChild) {
		this->doTree(copy, ts_tree_cursor_current_node(&copy));
	}

	auto copy2 = ts_tree_cursor_copy(&cursor);
	bool hasSibling = ts_tree_cursor_goto_next_sibling(&copy2);
	if (hasSibling) {
		this->doTree(copy2, ts_tree_cursor_current_node(&copy2));
	}
}

CodeView::Ptr CodeView::create(ScriptFile *scriptFile) {
	return std::make_shared<CodeView>(scriptFile);
}

CodeView::Ptr CodeView::create() { return std::make_shared<CodeView>(); }

void CodeView::setCode(ScriptFile *scriptFile) {
	this->scriptFile = scriptFile;

	/*
	auto parser = ts_parser_new();

	ts_parser_set_language(parser, tree_sitter_lua());
	auto fileContents = this->scriptFile->getFileContents().c_str();

	tsTree = ts_parser_parse_string(parser, nullptr, fileContents,
									strlen(fileContents));


	const auto rootNode = ts_tree_root_node(tsTree);
	const auto cursor = ts_tree_cursor_new(rootNode);

	this->doTree(cursor, rootNode);
	*/
}

void CodeView::drawCanvas() {
	ClearBackground(BLACK);
	auto fileContents = this->scriptFile->getFileContents();

	int x = 0, y = 0;

	for (auto m_char : fileContents) {
		DrawText(std::string(&m_char).c_str(), x, y, 20, WHITE);
	}
}

void CodeView::mouseMoved(tgui::Vector2f pos) { WorldView::mouseMoved(pos); }