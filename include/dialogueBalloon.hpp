#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <string>
#include <raylib.h>

struct DialogueLine {
	std::string characterName;
	std::string text;
	bool hasPortrait;
	Texture portrait;
};

class DialogueBalloon {
private:
	bool firstCharTyped;
	bool active;
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
	void showDialogue(DialogueLine line);
	void hideDialogue();
	void putChar(int charIndex);
	void putChar(int charIndex, Color color);
};

#endif