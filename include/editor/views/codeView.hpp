#ifndef _RPGPP_CODEVIEW_H
#define _RPGPP_CODEVIEW_H

#include "scriptFile.h"
#include "tree_sitter/api.h"
#include "views/worldView.hpp"
#include <list>
#include <memory>

struct HighlightInformation {};

class CodeView : public WorldView {

  private:
	ScriptFile *scriptFile;
	TSTree *tsTree;

  public:
	CodeView();
	CodeView(ScriptFile *scriptFile);

	typedef std::shared_ptr<CodeView> Ptr;

	static CodeView::Ptr create(ScriptFile *scriptFile);
	static CodeView::Ptr create();

	void setCode(ScriptFile *scriptFile);
	void mouseMoved(tgui::Vector2f pos) override;

	void drawCanvas() override;
	void doTree(TSTreeCursor cursor, TSNode node);
};

#endif /* _RPGPP_CODEVIEW_H */
