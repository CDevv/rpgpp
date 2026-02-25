#include "views/propView.hpp"
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
	auto ptr = std::make_shared<PropView>();
	ptr->p = p;
	return ptr;
}

void PropView::setProp(Prop* p) {
	this->p = p;
}

Prop* PropView::getProp() const {
	return this->p;
}

void PropView::drawCanvas() {
	if (this->p == nullptr) return;
	ClearBackground(RAYWHITE);
	Texture2D propTexture = p->getTexture();
	Rectangle atlasRect = p->getAtlasRect();
	Rectangle collisionRect = p->getCollisionRect();
	DrawTexturePro(
		propTexture,
		Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
		Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
		{0, 0}, 0, WHITE
	);
	atlasBox.updateRec(atlasRect);
	atlasBox.updateColor(Fade(BLUE, 0.5f));
	collisionBox.updateRec(collisionRect);
	collisionBox.updateColor(Fade(RED, 0.5f));

	atlasBox.draw();
	collisionBox.draw();
	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}

void PropView::drawOverlay() {
	if (this->p == nullptr) return;
}

void PropView::mouseMoved(tgui::Vector2f _) {
	if (this->p == nullptr) return;
	Vector2 mousePos = getMouseWorldPos();
	atlasBox.mouseMoved(mousePos);
	collisionBox.mouseMoved(mousePos);
	WorldView::mouseMoved(_);
}

bool PropView::leftMousePressed(tgui::Vector2f _) {
	if (this->p == nullptr) return false;
	Vector2 mousePos = getMouseWorldPos();
	atlasBox.leftMousePressed(mousePos);
	collisionBox.leftMousePressed(mousePos);
	return WorldView::leftMousePressed(_);
}

// void PropView::leftMouseReleased(tgui::Vector2f mousePos) {
// 	if (this->p == nullptr) return;
// 	atlasBox.leftMouseReleased(mousePos);
// 	collisionBox.leftMouseReleased(mousePos);
// 	WorldView::leftMouseReleased(mousePos);
// }
