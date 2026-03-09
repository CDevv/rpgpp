#ifndef _RPGPP_FILEVIEWS_SOUNDFILEVIEW_H
#define _RPGPP_FILEVIEWS_SOUNDFILEVIEW_H

#include "TGUI/Widgets/Label.hpp"
#include "fileViews/fileView.hpp"
#include "widgets/soundPlayer.hpp"

class SoundFileView : public FileView {
  public:
	SoundFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

  private:
	tgui::Label::Ptr infoLabel;
	SoundPlayer::Ptr soundPlayer;
};

#endif