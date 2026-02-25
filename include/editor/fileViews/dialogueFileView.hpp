#ifndef _RPGPP_FILEVIEWS_DIALOGUEFILEVIEW_H
#define _RPGPP_FILEVIEWS_DIALOGUEFILEVIEW_H

#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "fileViews/fileView.hpp"

class DialogueFileView : public FileView {
  private:
	tgui::GrowVerticalLayout::Ptr vertLayout;

  public:
	DialogueFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif