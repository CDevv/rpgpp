#include "fileViews/emptyView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "variant.hpp"

void EmptyFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	layout->add(tgui::Label::create("This is an EmptyFileView."));
}