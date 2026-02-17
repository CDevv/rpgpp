#include "fileViews/emptyView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "variant.hpp"

EmptyFileView::EmptyFileView() {
	auto noticeText = tgui::Label::create("No file opened or view not defined yet...");
	noticeText->setPosition("50%", "50%");
   	noticeText->setOrigin(0.5, 0.5);
	noticeText->setTextSize(24);
	widgetContainer.push_back(noticeText);
}

void EmptyFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	addWidgets(layout);
}
