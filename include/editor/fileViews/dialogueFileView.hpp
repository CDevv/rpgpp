#ifndef _RPGPP_FILEVIEWS_DIALOGUEFILEVIEW_H
#define _RPGPP_FILEVIEWS_DIALOGUEFILEVIEW_H

#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "dialogueBalloon.hpp"
#include "fileViews/fileView.hpp"
#include <cstddef>
#include <vector>

class DialogueFileView : public FileView {
	const float DIALOGUE_PANEL_HEIGHT = 200.0f;

  private:
	tgui::Button::Ptr newLineButton;
	tgui::ScrollablePanel::Ptr mainPanel;
	tgui::GrowVerticalLayout::Ptr vertLayout;
	tgui::Texture noImageTexture;
	tgui::Texture deleteTexture;
	std::vector<tgui::Panel::Ptr> linePanels;

	tgui::Panel::Ptr makeLinePanel(DialogueBin &data, DialogueLine line,
								   size_t i);

  public:
	DialogueFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif