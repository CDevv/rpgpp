#include "views/propPreview.hpp"

#include <memory>

#include "components/resizableCanvasBox.hpp"
#include "editor.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "services/translationService.hpp"

constexpr float MAX_BLINK_FREQ = 30;
constexpr int FONT_SIZE = 18;
constexpr Color OVER_BOUNDS_COLOR = {255, 255, 255, 200};

PropPreview::PropPreview() { camera.zoom = 3; }

void PropPreview::setProp(Prop *prop) { this->prop = prop; }

void PropPreview::setBox(ResizableCanvasBox *box) { this->referenceToBox = box; }

void PropPreview::drawCanvas() {
	ClearBackground(WHITE);
	TranslationService &ts = Editor::instance->getTranslations();

	auto texture = prop->getTexture();
	auto atlasRect = prop->getAtlasRect();
	auto boxRect = referenceToBox->getRectangle();

	this->isOverBounds = ((boxRect.x + boxRect.width > texture.width || boxRect.y + boxRect.height > texture.height) ||
						  (boxRect.x < 0 || boxRect.y < 0));
	drawOrigin();
	DrawTexturePro(texture, boxRect, {0, 0, boxRect.width, boxRect.height}, {0, 0}, 0,
				   {255, 255, 255, static_cast<unsigned char>(isOverBounds ? 100 : 255)});
}

void PropPreview::drawOverlay() {
	if (this->isOverBounds) DrawText("Over bounds!", 0, 0, FONT_SIZE, RED);

	DrawText("Preview", 0, this->getSize().y - FONT_SIZE, FONT_SIZE, BLACK);
}

PropPreview::Ptr PropPreview::create() { return std::make_shared<PropPreview>(); }

PropPreview::Ptr PropPreview::create(Prop *prop) {
	auto ptr = std::make_shared<PropPreview>();
	ptr->setProp(prop);
	return ptr;
}