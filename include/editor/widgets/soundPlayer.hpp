#ifndef _RPGPP_WIDGETS_SOUNDPLAYER_H
#define _RPGPP_WIDGETS_SOUNDPLAYER_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/Slider.hpp"
#include "saveables/soundWrapper.hpp"
#include "updatable.hpp"
#include <memory>

class SoundPlayer : public tgui::SubwidgetContainer, public IUpdatable {
  public:
	typedef std::shared_ptr<SoundPlayer> Ptr;

	tgui::Panel::Ptr panel;
	tgui::Slider::Ptr slider;
	std::weak_ptr<tgui::BitmapButton> playbackWeak;
	tgui::Label::Ptr timeLabel;

	SoundPlayer(const char *typeName = "SoundPlayer", bool initRenderer = true);

	static SoundPlayer::Ptr create();
	static SoundPlayer::Ptr copy(SoundPlayer::ConstPtr widget);

	void update() override;

	void setSize(const tgui::Layout2d &size) override;

	void setSound(SoundWrapper *sound);
	SoundWrapper *getSound();

  private:
	SoundWrapper *sound;
	void updateSize();

	tgui::Texture playTexture;
	tgui::Texture pauseTexture;

	bool isPlaying = false;
	bool mouseOnSlider = false;

  protected:
	Widget::Ptr clone() const override;
};

#endif