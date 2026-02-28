#include "views/actorView.hpp"
#include "TGUI/Vector2.hpp"
#include "actor.hpp"
#include "components/resizableCanvasBox.hpp"
#include "raylib.h"
#include "tileset.hpp"
#include "views/worldView.hpp"
#include <iostream>
#include <memory>
#include <string>

constexpr float DELTATIME_DIFFERENCE = 100.0f;

ActorView::ActorView() { camera.zoom = 5.0f; }

ActorView::Ptr ActorView::create() { return std::make_shared<ActorView>(); }

ActorView::Ptr ActorView::create(Actor *actor) {
	auto ptr = std::make_shared<ActorView>();
	ptr->setActor(actor);
	return ptr;
}

void ActorView::setActor(Actor *actor) {
	this->actor = actor;

	this->collisionBox = new ResizableCanvasBox(
		"collisionRect", this->actor->getCollisionRect(), RED);
}

void ActorView::mouseMoved(tgui::Vector2f pos) {
	if (this->actor == nullptr) return;

	collisionBox->mouseMoved(getMouseWorldPos());
	WorldView::mouseMoved(pos);
}


bool ActorView::leftMousePressed(tgui::Vector2f pos) {
	if (this->actor == nullptr) return false;

	if (collisionBox->leftMousePressed(getMouseWorldPos()))
		collisionBox->focused = true;

	return WorldView::leftMousePressed(pos);
}

void ActorView::leftMouseReleased(tgui::Vector2f pos) {
	if (this->actor == nullptr) return;

	actor->setCollisionRect(collisionBox->leftMouseReleased(getMouseWorldPos()));
	collisionBox->focused = false;

	WorldView::leftMouseReleased(pos);
}

void ActorView::drawCanvas() {

	if (actor == nullptr || collisionBox == nullptr) {
		std::cerr << "Importing Error!" << std::endl;
		return;
	}

	TileSet &tileSet = actor->getTileSet();

	if (animationCurrentDuration >
			animationFrameDuration / DELTATIME_DIFFERENCE &&
		isPlaying) {
		actor->update();
		animationCurrentDuration = 0.0f;
	}
	this->animationCurrentDuration += GetFrameTime();

	ClearBackground(RAYWHITE);
	actor->draw();
	collisionBox->draw();
	this->drawOrigin();

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}
