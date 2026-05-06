#ifndef _RPGPP_FILEVIEWS_UIFILEVIEW_H
#define _RPGPP_FILEVIEWS_UIFILEVIEW_H

#include "TGUI/Widgets/TreeView.hpp"
#include "fileViews/fileView.hpp"

class InterfaceViewFileView : public FileView {
private:
	static const int RIGHT_PANEL_W = 300;
	tgui::TreeView::Ptr treeView;

public:
	InterfaceViewFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif