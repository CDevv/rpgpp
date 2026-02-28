#include "fileViews/emptyView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "services/translationService.hpp"
#include "variant.hpp"

EmptyFileView::EmptyFileView() {
	TranslationService &ts = Editor::instance->getTranslations();
	auto noticeText =
		tgui::Label::create(ts.getKey("screen.project.emptyview.notice"));
	noticeText->setPosition("50%", "50%");
	noticeText->setOrigin(0.5, 0.5);
	noticeText->setTextSize(24);
	widgetContainer.push_back(noticeText);
}

void EmptyFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	addWidgets(layout);
}
