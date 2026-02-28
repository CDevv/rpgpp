#include "views/propView.hpp"
#include "components/resizableCanvasBox.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "views/worldView.hpp"
#include <memory>

PropView::PropView() {
	cameraMinZoom = 5.f;
	cameraMaxZoom = 20.f;
	camera.zoom = 5.f;
}

PropView::Ptr PropView::create() {
	return std::make_shared<PropView>();
}

PropView::Ptr PropView::create(Prop* p) {
	if (p == nullptr) return nullptr;
	auto ptr = std::make_shared<PropView>();
	ptr->p = p;
	ptr->setProp(p);

	return ptr;
}

void PropView::setProp(Prop* p) {
	if (p == nullptr) return;
	this->p = p;
	Rectangle atlasRect = p->getAtlasRect();
	Rectangle collisionRect = p->getCollisionRect();

	boxes.push_back(ResizableCanvasBox("atlasRect", atlasRect, BLUE));
	boxes.push_back(ResizableCanvasBox("collisionRect", collisionRect, RED));
}

Prop* PropView::getProp() const {
	return this->p;
}

void PropView::drawCanvas() {
	if (this->p == nullptr) return;
	ClearBackground(RAYWHITE);
	Texture2D propTexture = p->getTexture();

	DrawTexturePro(
		propTexture,
		Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
		Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
		{0, 0}, 0, WHITE
	);

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

void PropView::leftMouseReleased(tgui::Vector2f _) {
	if (this->p == nullptr) return;
	Vector2 mousePos = getMouseWorldPos();
	for (auto &box : boxes) {
		Rectangle newRect = box.leftMouseReleased(mousePos);
		// TODO: Should probably avoid using hard-coded string as ID, but for now, this will suffice.
		if (box.id == "atlasRect") {
			p->setAtlasRect(newRect);
		} else if (box.id == "collisionRect") {
			p->setCollisionRect(newRect);
		}
	}
	WorldView::leftMouseReleased(_);
}
