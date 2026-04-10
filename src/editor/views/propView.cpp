#include "views/propView.hpp"

#include <memory>

#include "components/resizableCanvasBox.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "views/worldView.hpp"

std::string rectTypeToStr(RectType type) {
	switch (type) {
		case ATLAS_RECT:
			return "atlasRect";
		case COLLISION_RECT:
			return "collisionRect";
		default:
			return "unknown";
	}
}

RectType rectTypeFromStr(const std::string &str) {
	if (str == "atlasRect") return ATLAS_RECT;
	if (str == "collisionRect") return COLLISION_RECT;
	return ATLAS_RECT;
}

PropView::PropView() {
	cameraMinZoom = 5.f;
	cameraMaxZoom = 20.f;
	camera.zoom = 5.f;
}

PropView::Ptr PropView::create() { return std::make_shared<PropView>(); }

PropView::Ptr PropView::create(Prop *p) {
	if (p == nullptr) return nullptr;
	auto ptr = std::make_shared<PropView>();
	ptr->p = p;
	ptr->setProp(p);

	return ptr;
}

void PropView::setProp(Prop *p) {
	if (p == nullptr) return;
	this->p = p;
	Rectangle atlasRect = p->getAtlasRect();
	Rectangle collisionRect = p->getCollisionRect();

	boxes.push_back(ResizableCanvasBox(rectTypeToStr(RectType::ATLAS_RECT), atlasRect, BLUE));
	boxes.push_back(ResizableCanvasBox(rectTypeToStr(RectType::COLLISION_RECT), collisionRect, RED));
}

Prop *PropView::getProp() const { return this->p; }

void PropView::drawCanvas() {
	if (this->p == nullptr) return;
	ClearBackground(RAYWHITE);
	Texture2D propTexture = p->getTexture();

	DrawTexturePro(propTexture, Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
				   Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f}, {0, 0}, 0, WHITE);

	this->drawOrigin();

	for (auto &box : boxes) {
		box.draw();
	}
	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}

void PropView::drawOverlay() {
	if (this->p == nullptr) return;
}

void PropView::mouseMoved(tgui::Vector2f _) {
	if (this->p == nullptr) return;
	Vector2 mousePos = getMouseWorldPos();
	for (auto &box : boxes) {
		box.mouseMoved(mousePos);
	}
	WorldView::mouseMoved(_);
}

bool PropView::leftMousePressed(tgui::Vector2f _) {
	if (this->p == nullptr) return false;
	Vector2 mousePos = getMouseWorldPos();
	auto focusedIt = boxes.end();

	for (auto it = boxes.begin(); it != boxes.end(); ++it) {
		it->focused = false;

		if (it->leftMousePressed(mousePos)) {
			focusedIt = it;
		}
	}

	if (focusedIt != boxes.end()) {
		focusedIt->focused = true;
		focusedBox = &(*focusedIt);
		boxes.splice(boxes.end(), boxes, focusedIt);
	}

	return WorldView::leftMousePressed(_);
}

void PropView::updateAtlasRect(Rectangle r) {
	if (this->p == nullptr) return;
	for (auto &box : boxes) {
		if (box.id == rectTypeToStr(RectType::ATLAS_RECT)) box.updateRec(r);
	}
	p->setAtlasRect(r);
}

void PropView::updateCollisionRect(Rectangle r) {
	if (this->p == nullptr) return;
	for (auto &box : boxes) {
		if (box.id == rectTypeToStr(RectType::COLLISION_RECT)) box.updateRec(r);
	}
	p->setCollisionRect(r);
}

void PropView::leftMouseReleased(tgui::Vector2f _) {
	if (this->p == nullptr) return;
	Vector2 mousePos = getMouseWorldPos();
	for (auto &box : boxes) {
		Rectangle newRect = box.leftMouseReleased(mousePos);
		switch (rectTypeFromStr(box.id)) {
			case ATLAS_RECT:
				p->setAtlasRect(newRect);
				onUpdatedAtlasRect.emit(this, newRect);
				break;
			case COLLISION_RECT:
				p->setCollisionRect(newRect);
				onUpdatedCollisionRect.emit(this, newRect);
				break;
			default:
				break;
		}
	}
	WorldView::leftMouseReleased(_);
}
