#ifndef RPGPP_ACTORVIEW_H
#define RPGPP_ACTORVIEW_H

#include "actor.hpp"
#include "components/resizableCanvasBox.hpp"
#include "views/worldView.hpp"
#include <memory>

constexpr float DEFAULT_ANIMATION_SPEED{2.0f};

class ActorView : public WorldView {
  private:
	ResizableCanvasBox *collisionBox;

	// Animation Editor Stuff.
	float animationFrameDuration{DEFAULT_ANIMATION_SPEED};
	float animationCurrentDuration{0.0f};

  public:
	ActorView();

	Actor *actor{nullptr};
	bool isPlaying{false};
	typedef std::shared_ptr<ActorView> Ptr;

	void setActor(Actor *actor);

	void drawCanvas() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;

	static ActorView::Ptr create();
	static ActorView::Ptr create(Actor *actor);
};

#endif
