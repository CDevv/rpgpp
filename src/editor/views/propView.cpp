#include "views/propView.hpp"
#include "prop.hpp"
#include "raylib.h"
#include <memory>

PropView::PropView() {}

PropView::Ptr create() {
	return std::make_shared<PropView>();
}

PropView::Ptr create(std::unique_ptr<Prop> p) {
	auto ptr = std::make_shared<PropView>();
	ptr->p = std::move(p);
	return ptr;
}

void PropView::setProp(std::unique_ptr<Prop> p) {
	this->p = std::move(p);
}

Prop* PropView::getProp() const {
	return p.get();
}

void PropView::drawCanvas() {
	if (!p) return;
	ClearBackground(WHITE);
}

void PropView::drawOverlay() {
	if (!p) return;
	// Draw overlay
}

void PropView::mouseMoved(tgui::Vector2f mousePos) {
	if (!p) return;
	// Handle mouse movement
}

bool PropView::leftMousePressed(tgui::Vector2f mousePos) {
	if (!p) return false;
	// Handle left mouse button press
}
