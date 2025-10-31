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
	Dialogue dialogue;
	std::string text;
	DialogueTextSection sectionText;
	bool firstCharTyped;
	bool active;
	int frameCounter;
	int charIndex;
	int lineIndex;
	int sectionIndex;
	Color textColor;
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
	void charP(Vector2 charMeasure, const char* c, Color color);
};

#endif
