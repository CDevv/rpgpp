#ifndef _RPGPP_EDITOR_ANIMATIONSVIEW_H
#define _RPGPP_EDITOR_ANIMATIONSVIEW_H

#include <raylib.h>
#include <vector>
#include <string>
#include "actor.hpp"
#include "actorView.hpp"

class AnimationsView {
private:
	ActorView* actorView;
	Rectangle rect;
	bool animDropdownActive;
	bool animDropdownEditMode;
	int currentAnim;
	bool animPlaying;
	std::vector<Vector2> animFrames;
	std::vector<std::string> animNames;
public:
	AnimationsView();
	AnimationsView(Rectangle rect);
	void update();
	void draw();
	void setActorView(ActorView* actorView);
	int getCurrentAnim();
	bool getAnimPlaying();
};

#endif