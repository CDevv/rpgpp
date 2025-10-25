#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <string>
#include <vector>
#include <raylib.h>

struct DialogueLine {
	std::string characterName;
	std::string text;
	bool hasPortrait;
};

struct Dialogue {
    std::string title;
	std::vector<DialogueLine> lines;
};

class DialogueBalloon {
private:
	Rectangle rect;
	Rectangle textRect;
	bool firstCharTyped;
	bool active;
	int frameCounter;
	int charIndex;
	int rowIndex;
	int colIndex;
	int lineTextStart;
	Dialogue dialogue;
	int lineIndex;
	std::string text;
	std::string lineText;
public:
	DialogueBalloon();
	DialogueBalloon(Rectangle rect);
	void update();
	void draw();
	void showDialogue(Dialogue dialogue);
	void hideDialogue();
	void putChar(int charIndex);
	void putChar(int charIndex, Color color);
};

#endif
