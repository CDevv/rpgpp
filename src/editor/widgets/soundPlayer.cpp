#include "widgets/soundPlayer.hpp"

#include <cstdio>

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "saveables/soundWrapper.hpp"
#include "timeFormat.hpp"

SoundPlayer::SoundPlayer(const char *typeName, bool initRenderer) : tgui::SubwidgetContainer(typeName, initRenderer) {
	playTexture = tgui::Texture(Editor::instance->getFs().getResourcePath("play.png"));
	pauseTexture = tgui::Texture(Editor::instance->getFs().getResourcePath("pause.png"));

	panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({4, 4});

	slider = tgui::Slider::create();
	slider->setPosition("parent.height + 12", 4 + 4);
	slider->setSize({"80% - 16", "100% - 16"});

	slider->onValueChange([this](float value) {
		if (mouseOnSlider && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			SeekMusicStream(sound->sound, value);
		}
	});
	slider->onMouseEnter([this] { mouseOnSlider = true; });
	slider->onMouseLeave([this] { mouseOnSlider = false; });

	panel->add(slider);

	auto playbackButton = tgui::BitmapButton::create();
	playbackButton->setPosition(0, 0);
	playbackButton->setSize("height", "100%");
	playbackButton->setImage(playTexture);

	playbackWeak = playbackButton;

	playbackButton->onClick([&] {
		if (isPlaying) {
			playbackWeak.lock()->setImage(playTexture);
			PauseMusicStream(sound->sound);
		} else {
			playbackWeak.lock()->setImage(pauseTexture);
			if (IsMusicValid(sound->sound)) {
				if (slider->getValue() > 0) {
					if (hasPlayed) {
						ResumeMusicStream(sound->sound);
					} else {
						PlayMusicStream(sound->sound);
						SeekMusicStream(sound->sound, slider->getValue());
					}
				} else {
					PlayMusicStream(sound->sound);
					hasPlayed = true;
				}
			} else {
				printf("seems this music is invalid.. \n");
			}
		}
		isPlaying = !isPlaying;
	});

	panel->add(playbackButton);

	timeLabel = tgui::Label::create();
	timeLabel->setPosition("80% + parent.height + 18", 0);
	timeLabel->setSize("20% - parent.height - 8", "100%");
	timeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);

	panel->add(timeLabel);

	m_container->add(panel);

	updateSize();
}

void SoundPlayer::setSound(SoundWrapper *sound) {
	this->sound = sound;
	slider->setMaximum(GetMusicTimeLength(sound->sound));
	slider->setMinimum(0.0f);
	slider->setStep(1.0f);
	slider->setValue(0.0f);

	timeLabel->setText(TextFormat("0:00 | %s", formatTime(GetMusicTimeLength(sound->sound)).c_str()));
}

SoundWrapper *SoundPlayer::getSound() { return sound; }

SoundPlayer::Ptr SoundPlayer::create() { return std::make_shared<SoundPlayer>(); }

SoundPlayer::Ptr SoundPlayer::copy(SoundPlayer::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<SoundPlayer>(widget->clone());
	else
		return nullptr;
}

void SoundPlayer::update() {
	if (isPlaying && IsMusicValid(sound->sound)) {
		UpdateMusicStream(sound->sound);

		slider->setValue(GetMusicTimePlayed(sound->sound));

		timeLabel->setText(TextFormat("%s | %s", formatTime(GetMusicTimePlayed(sound->sound)).c_str(),
									  formatTime(GetMusicTimeLength(sound->sound)).c_str()));
	}
}

tgui::Widget::Ptr SoundPlayer::clone() const { return std::make_shared<SoundPlayer>(*this); }

void SoundPlayer::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);

	updateSize();
}

void SoundPlayer::updateSize() { panel->setSize({getSize().x, getSize().y}); }