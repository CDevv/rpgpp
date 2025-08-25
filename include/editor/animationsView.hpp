#ifndef _RPGPP_EDITOR_ANIMATIONSVIEW_H
#define _RPGPP_EDITOR_ANIMATIONSVIEW_H

#include <raylib.h>
#include <vector>
#include <string>

class AnimationsView {
private:
	Rectangle rect;
	bool animDropdownActive;
	bool animDropdownEditMode;
	int currentAnim;
	std::vector<Vector2> animFrames;
	std::vector<std::string> animNames;
public:
	AnimationsView();
	AnimationsView(Rectangle rect);
	void update();
	void draw();
};

#endif