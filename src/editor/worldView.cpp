#include "worldView.hpp"
#include "TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp"
#include "TGUI/Backend/Window/BackendGui.hpp"
#include "TGUI/Widget.hpp"

#include "raylib.h"
#include "updatable.hpp"
#include <TGUI/Backend/Renderer/Raylib/BackendRendererRaylib.hpp>
#include <TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp>
#include <TGUI/Backend/Window/Backend.hpp>
#include <memory>

WorldView::WorldView(const char *typeName, bool initRenderer)
	: tgui::CanvasRaylib(typeName, initRenderer) {
	canvas = tgui::CanvasRaylib::create();
	canvas->setPosition(getPosition());

	auto size = getSize();
	// texture =
	//	LoadRenderTexture(static_cast<int>(size.x), static_cast<int>(size.y));
	//  getParentGui()->add(canvas);
}

// WorldView::~WorldView() { UnloadRenderTexture(texture); }

WorldView::Ptr WorldView::create() { return std::make_shared<WorldView>(); }

WorldView::Ptr WorldView::copy(WorldView::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<WorldView>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr WorldView::clone() const {
	return std::make_shared<WorldView>(*this);
}

void WorldView::setSize(const tgui::Layout2d &size) {
	tgui::CanvasRaylib::setSize(size);
}

bool WorldView::isMouseOnWidget(tgui::Vector2f pos) const {
	return tgui::FloatRect{getPosition().x, getPosition().y, getSize().x,
						   getSize().y}
		.contains(pos);
}

void WorldView::update() {
	BeginTextureMode(m_textureTarget);

	ClearBackground(RAYWHITE);

	DrawRectangle(0, 0, 200, 100, YELLOW);

	EndTextureMode();
}

void WorldView::draw(tgui::BackendRenderTarget &target,
					 tgui::RenderStates states) const {
	const tgui::Borders borders{2}; // Borders are 2 pixels thick on any side

	tgui::CanvasRaylib::draw(target, states);
	target.drawBorders(
		states, borders, getSize(),
		tgui::Color::applyOpacity(tgui::Color::White, m_opacityCached));
}

std::shared_ptr<IUpdatable>
WorldView::asUpdatable(std::shared_ptr<WorldView> ptr) {
	return std::dynamic_pointer_cast<IUpdatable>(ptr);
}