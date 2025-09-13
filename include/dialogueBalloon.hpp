#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <string>
#include <raylib.h>

class DialogueBalloon {
private:
	int frameCounter;
	int charIndex;
	int rowIndex;
	int colIndex;
	int lineTextStart;
	Rectangle rect;
	Rectangle textRect;
	std::string text;
	std::string lineText;
public:
	DialogueBalloon();
	DialogueBalloon(Rectangle rect);
	void update();
	void draw();
	void putChar(int charIndex);
	void putChar(int charIndex, Color color);
};

#endif