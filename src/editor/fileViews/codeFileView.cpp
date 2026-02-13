#include "fileViews/codeFileView.hpp"

#include "scriptFile.h"
#include "widgets/codeEditor.hpp"

#include "tree_sitter/tree-sitter-lua.h"
#include <cstdio>
#include <tree_sitter/api.h>

void do_tree_node(TSTreeCursor cursor, TSNode node) {
	printf("====== \n");
	printf("type: %s \n", ts_node_type(node));
	printf("start byte: %u \n", ts_node_start_byte(node));
	printf("end byte: %u \n", ts_node_end_byte(node));
	printf("====== \n");

	auto copy = ts_tree_cursor_copy(&cursor);
	bool hasChild = ts_tree_cursor_goto_first_child(&copy);
	if (hasChild) {
		do_tree_node(copy, ts_tree_cursor_current_node(&copy));
	}

	auto copy2 = ts_tree_cursor_copy(&cursor);
	bool hasSibling = ts_tree_cursor_goto_next_sibling(&copy2);
	if (hasSibling) {
		do_tree_node(copy2, ts_tree_cursor_current_node(&copy2));
	}
}

CodeFileView::CodeFileView() {
	codeEditor = CodeEditor::create();
	codeEditor->setSize({"100%", "100%"});
	widgetContainer.push_back(codeEditor);
}

void CodeFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<ScriptFile> *>(variant);

		TSParser *parser = ts_parser_new();

		ts_parser_set_language(parser, tree_sitter_lua());

		auto tree = ts_parser_parse_string(
			parser, nullptr, ptr->get()->getFileContents().c_str(),
			ptr->get()->getFileContents().size());

		auto root = ts_tree_root_node(tree);

		auto cursor = ts_tree_cursor_new(root);
		do_tree_node(cursor, root);

		codeEditor->setText(ptr->get()->getFileContents());
		codeEditor->setTextSize(24);
		addWidgets(layout);

		ts_parser_delete(parser);
	}
}
