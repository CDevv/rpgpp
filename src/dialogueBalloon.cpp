#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "interfaceService.hpp"

DialogueBalloon::DialogueBalloon() {}

DialogueBalloon::DialogueBalloon(Rectangle rect)
{
	this->frameCounter = 0;
	this->charIndex = 0;
	this->rowIndex = 0;
	this->colIndex = 0;
	this->lineTextStart = 0;
	this->rect = rect;
	this->textRect = Rectangle {
		rect.x + 9, rect.y + 9, rect.width - (9 * 2), rect.height - (9 * 2)
	};
	this->text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec placerat vel nulla eget ullamcorper. Proin varius erat in tristique dignissim. Aliquam erat volutpat.";
	this->lineText = text;
}

void DialogueBalloon::update()
{
	frameCounter++;
	if (frameCounter > (60/20)) {
		frameCounter = 0;
		if (charIndex < text.size()) {
			charIndex++;
		}
	}
}

void DialogueBalloon::draw()
{
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

		float resultX = (textRect.x + stringSize.x);
		if ((textRect.x + stringSize.x) > (textRect.x + textRect.width)) {
			lineTextStart = charIndex;
			resultX = textRect.x;
			rowIndex++;
		}

		Vector2 charPosition = Vector2 { resultX, textRect.y + (stringSize.y * rowIndex)};
		DrawTextEx(font, TextSubtext(text.c_str(), charIndex, 1), charPosition, 13 * 3, 1, color);
	}
}