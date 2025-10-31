#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <string>
#include <vector>
#include <raylib.h>

struct DialogueTextSection {
    std::string key;
    std::string text;
};

struct DialogueLine {
	std::string characterName;
	std::string text;
	bool hasPortrait;
	std::string imageId;
	std::vector<DialogueTextSection> sections;
};

struct Dialogue {
    std::string title;
	std::vector<DialogueLine> lines;
};

class DialogueBalloon {
private:
	Rectangle rect;
	Rectangle textRect;
	Rectangle textPortraitRect;
	bool firstCharTyped;
	bool active;
	int frameCounter;
	int charIndex;
	int rowIndex;
	int colIndex;
	int lineTextStart;
	Dialogue dialogue;
	int lineIndex;
	int sectionIndex;
	Color textColor;
	std::string text;
	std::string lineText;
	DialogueTextSection sectionText;
	int lastSectionLen;
	Vector2 textPos;
	
	void drawPortrait();
public:
	DialogueBalloon();
	DialogueBalloon(Rectangle rect);
	void update();
	void draw();
	void showDialogue(Dialogue dialogue);
	void hideDialogue();
	void putChar(int charIndex);
	void putChar(int charIndex, Color color);
	void charP(Vector2 charMeasure, const char* c, Color color);
};

#endif
