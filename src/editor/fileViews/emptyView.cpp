#include "fileViews/emptyView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "variant.hpp"

EmptyFileView::EmptyFileView() {
	widgetContainer.push_back(tgui::Label::create("This is an EmptyFileView."));
}

void EmptyFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	addWidgets(layout);
}