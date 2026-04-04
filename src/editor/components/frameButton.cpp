#include "components/frameButton.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Rect.hpp"
#include "TGUI/Sprite.hpp"
#include "TGUI/Texture.hpp"
#include "actor.hpp"
#include "components/tooltip.hpp"
#include "raylib.h"
#include <memory>
#include <string>

constexpr int CIRCLE_RADIUS = 10;
constexpr float PADDING = 10.0f;

FrameButton::FrameButton(const int frameIndex, Actor *actor)
	: frameIndex(frameIndex), actor(actor) {

	this->setMouseCursor(tgui::Cursor::Type::Hand);

	this->ButtonBase::setSize({50, "100%"});
	this->updateSprite();

	this->setToolTip(Tooltip::create(std::to_string(frameIndex)));

	this->onPress.connect(
		[this] { this->onFrameChange.emit(this, this->frameIndex); });
}

FrameButton::Ptr FrameButton::create(int frameIndex, Actor *actor) {
	return std::make_shared<FrameButton>(frameIndex, actor);
}

void FrameButton::updateSprite(bool reImport) {
	auto &tileset = actor->getTileSet();
	auto [x, y] = actor->getAnimationAtlasByIdx(this->frameIndex);

	const auto &texture = tileset.getTexture();

	if (reImport || !this->sprite.isSet()) {
		// TGUI keeping the size of the previously imported sprite kinda messes
		// it up, so fix.
		sprite.setSize({static_cast<float>(texture.width),
						static_cast<float>(texture.height)});

		sprite.setTexture(tgui::Texture{tileset.getTextureSource()});
	}

	auto width = static_cast<float>(tileset.getTileWidth()),
		  height = static_cast<float>(tileset.getTileHeight());
	float xPos = x * width, yPos = y * height;

	sprite.setVisibleRect({
		xPos,
		yPos,
		width,
		height,
	});

	sprite.setPosition({-xPos - width / 2.0f, -yPos - height / 2.0f});
}

void FrameButton::draw(tgui::BackendRenderTarget &target,
					   tgui::RenderStates states) const {

	Button::draw(target, states);

	float buttonWidth = m_size.x.getValue(), buttonHeight = m_size.y.getValue();

	target.drawBorders(states, 2, {buttonWidth, buttonHeight},
					   actor->getCurrentFrame() == this->frameIndex
						   ? tgui::Color::Blue
						   : tgui::Color::Black);

	const tgui::FloatRect visibleRect = this->sprite.getVisibleRect();

	states.transform.translate({buttonWidth / 2.0f, buttonHeight / 2.0f});
	states.transform.scale({((buttonWidth - PADDING) / visibleRect.width),
							((buttonHeight - PADDING) / visibleRect.height)});
	target.drawSprite(states, this->sprite);
}
