#ifndef _RPGPP_EDITOR_TEXTAREA_H
#define _RPGPP_EDITOR_TEXTAREA_H

#include <raylib.h>
#include <string>

enum TextAreaStatus {
	TEXTAREA_NORMAL, TEXTAREA_HOVER, TEXTAREA_FOCUS
};

class TextArea {
private:
	TextAreaStatus state;
	Rectangle rect;
	std::string content;
	int linesCount;
	Rectangle viewRec;
	Vector2 scrollVec;
	Vector2 cursorPos;
	void keyInput();
public:
	TextArea();
	TextArea(Rectangle rect);
	void update();
	void draw();
};

#endif