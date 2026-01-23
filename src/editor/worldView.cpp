#include "worldView.hpp"
#include "TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp"
#include "TGUI/Backend/Window/BackendGui.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"

#include "raylib.h"
#include "raymath.h"
#include "updatable.hpp"
#include <TGUI/Backend/Renderer/Raylib/BackendRendererRaylib.hpp>
#include <TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp>
#include <TGUI/Backend/Window/Backend.hpp>
#include <cstdio>
#include <memory>

WorldView::WorldView(const char *typeName, bool initRenderer)
	: tgui::CanvasRaylib(typeName, initRenderer) {
	mouseMiddleButton = false;

	tgui::Vector2f size = getSize();
	texture =
		LoadRenderTexture(static_cast<int>(size.x), static_cast<int>(size.y));

	camera = {};
	camera.offset = Vector2{0, 0};
	camera.rotation = 0.0f;
	camera.target = Vector2{0, 0};
	camera.zoom = 1.0f;
}

WorldView::Ptr WorldView::create() { return std::make_shared<WorldView>(); }

WorldView::Ptr WorldView::copy(const ConstPtr &widget) {
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

	UnloadRenderTexture(texture);
	tgui::Vector2f newSize = getSize();
	texture = LoadRenderTexture(static_cast<int>(newSize.x),
								static_cast<int>(newSize.y));
}

bool WorldView::isMouseOnWidget(tgui::Vector2f pos) const {
	return tgui::FloatRect{getPosition().x, getPosition().y, getSize().x,
						   getSize().y}
		.contains(pos);
}

void WorldView::mouseMoved(tgui::Vector2f pos) {
	Vector2 mouseDelta = GetMouseDelta();

	if (mouseMiddleButton) {
		Vector2 cameraMoveScale = Vector2Scale(mouseDelta, -1 / camera.zoom);
		camera.target = Vector2Add(camera.target, cameraMoveScale);
	}

	tgui::Widget::mouseMoved(pos);
}

bool WorldView::scrolled(float delta, tgui::Vector2f pos, bool touch) {
	if (!touch) {
		Vector2 mouseWorldPos =
			GetScreenToWorld2D(Vector2{pos.x, pos.y}, camera);
		camera.offset = Vector2{pos.x, pos.y};
		camera.target = mouseWorldPos;

		float scale = delta * 0.2f;
		camera.zoom = Clamp(camera.zoom + scale, 1.0f, 5.0f);
	}

	return Widget::scrolled(delta, pos, touch);
}

void WorldView::update() {
	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
		mouseMiddleButton = true;
	} else {
		mouseMiddleButton = false;
	}

	BeginTextureMode(m_textureTarget);
	BeginMode2D(camera);

	drawCanvas();

	EndMode2D();
	EndTextureMode();
}

void WorldView::drawCanvas() {
	ClearBackground(RAYWHITE);

	DrawRectangle(0, 0, 200, 100, YELLOW);
}

void WorldView::draw(tgui::BackendRenderTarget &target,
					 const tgui::RenderStates states) const {
	constexpr tgui::Borders borders{
		2}; // Borders are 2 pixels thick on any side

	CanvasRaylib::draw(target, states);
	target.drawBorders(
		states, borders, getSize(),
		tgui::Color::applyOpacity(tgui::Color::White, m_opacityCached));
}

std::shared_ptr<IUpdatable>
WorldView::asUpdatable(const std::shared_ptr<WorldView> &ptr) {
	return std::dynamic_pointer_cast<IUpdatable>(ptr);
}