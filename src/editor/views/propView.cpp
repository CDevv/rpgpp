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
	DrawTexturePro(
		propTexture,
		Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
		Rectangle{0, 0, propTexture.width * 1.f, propTexture.height * 1.f},
		{0, 0}, 0, WHITE
	);
	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}

void PropView::drawOverlay() {
	if (this->p == nullptr) return;
}

void PropView::mouseMoved(tgui::Vector2f mousePos) {
	if (this->p == nullptr) return;
	WorldView::mouseMoved(mousePos);
}

bool PropView::leftMousePressed(tgui::Vector2f mousePos) {
	if (this->p == nullptr) return false;		// Handle left mouse button press
	return WorldView::leftMousePressed(mousePos);
}
