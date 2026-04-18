#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <raylib.h>

#include <string>
#include <vector>

struct DialogueTextSection {
	std::string key;
	std::string text;
	Color textColor = WHITE;
	int textSize = 13;
	std::string font = "LanaPixel";
	float delay = 0.0f;
};

struct DialogueOption {
	std::string title;
	std::string nextDialogue;
};

struct DialogueLine {
	std::string characterName;
	std::string text;
	bool hasPortrait;
	std::string imageId;
	std::vector<DialogueTextSection> sections;
	bool hasOptions = false;
	std::vector<DialogueOption> options;
};

struct DialogueBin {
	std::string title;
	std::vector<DialogueLine> lines;
};

class DialogueBalloon {
private:
	std::string fontName = "LanaPixel";
	Font font;
	Rectangle rect;
	Rectangle textRect;
	Rectangle textPortraitRect;
	Rectangle optionsRect;
	DialogueBin dialogue;
	std::string text;
	DialogueTextSection sectionText;
	bool firstCharTyped;
	bool finishedTyping = false;
	bool active;
	int frameCounter;
	int charIndex;
	int lineIndex;
	int sectionIndex;
	Color textColor;
	int lastSectionLen;
	Vector2 textPos;
	bool delay = false;
	float delayDuration = 0.0f;
	int hoveredOption = 0;
	void drawPortrait() const;

public:
	DialogueBalloon();
	DialogueBalloon(Rectangle rect);
	void update();
	void draw();
	void showDialogue(const DialogueBin &newDialogue);
	void hideDialogue();
	void charP(Vector2 charMeasure, const char *c, DialogueLine &textLine, DialogueTextSection &textSection);
};

#endif
