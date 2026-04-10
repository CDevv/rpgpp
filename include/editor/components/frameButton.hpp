#ifndef RPGPP_FRAMEBUTTON_H
#define RPGPP_FRAMEBUTTON_H

#include <memory>

#include "TGUI/Signal.hpp"
#include "TGUI/Sprite.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "actor.hpp"
class FrameButton : public tgui::Button {
private:
	int frameIndex;
	Actor *actor;
	tgui::Sprite sprite{};

public:
	FrameButton(int frameIndex, Actor *actor);

	typedef std::shared_ptr<FrameButton> Ptr;
	static FrameButton::Ptr create(int frameIndex, Actor *actor);

	void draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const override;
	void updateSprite(bool reImport = false);

	// When the frame button is pressed. Optional parameters: frame index
	tgui::SignalInt onFrameChange = {"OnFrameChange"};
};

#endif	// RPGPP_FRAMEBUTTON_H
