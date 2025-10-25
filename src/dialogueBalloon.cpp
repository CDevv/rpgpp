#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "interfaceService.hpp"
#include <nlohmann/json.hpp>
#include <raylib.h>

DialogueBalloon::DialogueBalloon() {}

DialogueBalloon::DialogueBalloon(Rectangle rect)
{
	this->rect = rect;
	this->textRect = Rectangle {
		rect.x + 9, rect.y + 9, rect.width - (9 * 2), rect.height - (9 * 2)
	};

	this->lineIndex = 0;

	this->firstCharTyped = false;
	this->active = false;
	this->frameCounter = 0;
	this->charIndex = 0;
	this->rowIndex = 0;
	this->colIndex = 0;
	this->lineTextStart = 0;

	this->text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec placerat vel nulla eget ullamcorper. Proin varius erat in tristique dignissim. Aliquam erat volutpat.";
	this->lineText = text;
}

void DialogueBalloon::update()
{
	if (active) {
		if (firstCharTyped == false) {
			firstCharTyped = true;
			return;
		}

		bool finished = false;
		if (charIndex == (text.size() - 1) || charIndex == text.size()) {
			finished = true;
		}

		if (IsKeyPressed(KEY_Z)) {
			if (finished) {
				if (lineIndex == (dialogue.lines.size() - 1)) {
					hideDialogue();
				} else {
					lineIndex++;
					text = dialogue.lines.at(lineIndex).text;
					charIndex = 0;
					rowIndex = 0;
					firstCharTyped = false;
				}
			} else {
				charIndex = (text.size() - 1);
			}
		}

		frameCounter++;
		if (frameCounter > (60/20)) {
			frameCounter = 0;
			if (charIndex < text.size()) {
				charIndex++;
			}
		}
	}
}

void DialogueBalloon::draw()
{
	if (active) {
	    DrawTexture(Game::getResources().getTexture("Hills.png"), 20, 20, WHITE);

		Font font = Game::getUi().getFont();
		Texture uiTexture = Game::getUi().getTexture();

		Vector2 origin = { 0.0f, 0.0f };
		NPatchInfo info = NPatchInfo {
	        Rectangle { 0, 0, static_cast<float>(uiTexture.width), static_cast<float>(uiTexture.height) },
	        3 * 3, 3 * 3, uiTexture.width - (3 * 3), uiTexture.height - (3 * 3)
	    };

	    DrawTextureNPatch(uiTexture, info, rect, origin, 0.0f, WHITE);

	    lineText = text;
	    lineTextStart = 0;
	    rowIndex = 0;
	    for (int i = 0; i < charIndex; i++) {
	    	putChar(i);
    	}
	}
}

void DialogueBalloon::showDialogue(Dialogue dialogue)
{
	if (active) return;

	this->dialogue = dialogue;
	this->lineIndex = 0;

	firstCharTyped = false;
	active = true;
	this->text = dialogue.lines.at(0).text;
	this->lineText = text;

	this->frameCounter = 0;
	this->charIndex = 0;
	this->rowIndex = 0;
}

void DialogueBalloon::hideDialogue()
{
	active = false;
}

void DialogueBalloon::putChar(int charIndex)
{
	putChar(charIndex, WHITE);
}

void DialogueBalloon::putChar(int charIndex, Color color)
{
	lineText = TextSubtext(text.c_str(), lineTextStart, (charIndex + 1) - lineTextStart);

	Font font = Game::getUi().getFont();
	if (charIndex == 0) {
		DrawTextEx(font, TextSubtext(text.c_str(), lineTextStart, 1), Vector2 { textRect.x, textRect.y }, 13 * 3, 1, color);
	} else {
		colIndex++;
		Vector2 stringSize = MeasureTextEx(font, TextSubtext(text.c_str(), lineTextStart, charIndex - lineTextStart), 13 * 3, 1.0f);
		Vector2 singleCharSize = MeasureTextEx(font, TextSubtext(text.c_str(), charIndex, 1), 13 * 3, 1.0f);

		float resultX = (textRect.x + stringSize.x);
		if ((textRect.x + stringSize.x + singleCharSize.x) > (textRect.x + textRect.width)) {
			lineTextStart = charIndex;
			resultX = textRect.x;
			rowIndex++;
		}

		Vector2 charPosition = Vector2 { resultX, textRect.y + (stringSize.y * rowIndex)};
		DrawTextEx(font, TextSubtext(text.c_str(), charIndex, 1), charPosition, 13 * 3, 1, color);
	}
}
