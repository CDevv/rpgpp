#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <raylib.h>

class DialogueBalloon {
private:
	Rectangle rect;
public:
	DialogueBalloon();
	DialogueBalloon(Rectangle rect);
	void draw();
};

#endif