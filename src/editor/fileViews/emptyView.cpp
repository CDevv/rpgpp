#include "fileViews/emptyView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"

void EmptyFileView::init(tgui::Group::Ptr layout) {
	layout->add(tgui::Label::create("This is an EmptyFileView."));
}