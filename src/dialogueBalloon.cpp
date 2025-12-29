#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "interfaceService.hpp"
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <raymath.h>

DialogueBalloon::DialogueBalloon() {}

DialogueBalloon::DialogueBalloon(Rectangle rect)
{
	this->rect = rect;
	this->textRect = Rectangle {
		rect.x + 9, rect.y + 9, rect.width - (9 * 2), rect.height - (9 * 2)
	};
	this->textPortraitRect = this->textRect;
	this->textPortraitRect.width -= this->textRect.height;
	this->textPortraitRect.x += this->textRect.height;

	this->lineIndex = 0;
	this->sectionIndex = 0;

	this->firstCharTyped = false;
	this->active = false;
	this->frameCounter = 0;
	this->charIndex = 0;

	this->textColor = WHITE;
	this->text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec placerat vel nulla eget ullamcorper. Proin varius erat in tristique dignissim. Aliquam erat volutpat.";
	this->sectionText = { "", text };
	this->lastSectionLen = 0;

	this->textPos = Vector2 { 0, 0 };
}

void DialogueBalloon::update()
{
	if (active) {
		if (firstCharTyped == false) {
			firstCharTyped = true;
			Game::getSounds().playSound("Text 1.wav");
			return;
		}

		bool finished = false;
		if (charIndex == (text.size() - 1) ||
		charIndex == text.size()) {
			finished = true;
		}

		if (IsKeyPressed(KEY_Z)) {
			if (finished) {
				if (lineIndex == (dialogue.lines.size() - 1)) {
					hideDialogue();
				} else {
				    lineIndex++;
                    text = "";
    				for (auto k : dialogue.lines.at(lineIndex).sections) {
    				    text = text.append(k.text);
    				}

					charIndex = 0;
					firstCharTyped = false;
					sectionIndex = 0;
					lastSectionLen = 0;

					textPos = Vector2 { 0, 0 };
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

				//play sound
				if (charIndex < text.size()) {
                    if (text.at(charIndex) != ' ') {
				        Game::getSounds().playSound("Text 1.wav");
				    }
				}
			}
		}
	}
}

void DialogueBalloon::drawPortrait()
{
    Texture2D texture = Game::getResources().getTexture(dialogue.lines.at(lineIndex).imageId);
	Rectangle source = Rectangle { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	Rectangle dest = Rectangle { rect.x + 9, rect.y + 9, rect.height - (9 * 2), rect.height - (9 * 2) };
	DrawTexturePro(texture, source, dest, Vector2 { 0, 0 }, 0.0f, WHITE);
}

void DialogueBalloon::draw()
{
	if (active) {
		Font font = Game::getUi().getFont();
		Texture uiTexture = Game::getUi().getTexture();

		Vector2 origin = { 0.0f, 0.0f };
		NPatchInfo info = NPatchInfo {
	        Rectangle { 0, 0, static_cast<float>(uiTexture.width), static_cast<float>(uiTexture.height) },
	        3 * 3, 3 * 3, uiTexture.width - (3 * 3), uiTexture.height - (3 * 3)
	    };

	    DrawTextureNPatch(uiTexture, info, rect, origin, 0.0f, WHITE);

		if (dialogue.lines.at(lineIndex).hasPortrait) {
		    drawPortrait();
		}

		textPos = Vector2{ 0, 0 };
		sectionIndex = 0;
		Vector2 charMeasure = Vector2 { 0, 0 };
	    for (int i = 0; i < charIndex; i++) {
			Color textColor = WHITE;

			int size = 0;
    		int idx = 0;
    		for (auto section : dialogue.lines.at(lineIndex).sections) {
    		    if (i < (size + TextLength(section.text.c_str()))) {
    				sectionIndex = idx;
                    if (section.key == "red") {
                        textColor = RED;
                    } else if (section.key == "blue") {
                        textColor = BLUE;
                    } else if (section.key == "green") {
                        textColor = GREEN;
                    }
                    break;
    			} else {
    			    size += TextLength(section.text.c_str());
    			}
    			idx++;
    		}

            const char* subText = TextSubtext(text.c_str(), i, 1);
			charP(charMeasure, subText, textColor);

			Vector2 newMeasure = MeasureTextEx(font,
			    TextSubtext(text.c_str(), i, 1), 13 * 3, 1.0f);
			charMeasure = newMeasure;
    	}
	}
}

void DialogueBalloon::charP(Vector2 charMeasure, const char* c, Color color)
{
    Font font = Game::getUi().getFont();
    Rectangle resRect = this->textRect;
    if (dialogue.lines.at(lineIndex).hasPortrait) {
        resRect = textPortraitRect;
    }

    Vector2 a = Vector2Add(textPos, Vector2 { charMeasure.x + 1 , 0.0f });
    Vector2 finalCharPos = Vector2Add(Vector2 { resRect.x, resRect.y }, a);

    //Check for text overflow on x axis.
    if ((finalCharPos.x + charMeasure.x) > textRect.width) {
        textPos.x = 0;
        textPos.y += charMeasure.y;

        a = textPos;
        finalCharPos = Vector2Add(Vector2 { resRect.x, resRect.y }, a);
    }
    else {
        textPos.x += charMeasure.x;
    }

    DrawTextEx(font, c, finalCharPos, 13 * 3, 1, color);
}

void DialogueBalloon::showDialogue(Dialogue dialogue)
{
	if (active) return;

	this->dialogue = dialogue;
	this->lineIndex = 0;

	firstCharTyped = false;
	active = true;
	this->text = dialogue.lines.at(0).text;

	this->frameCounter = 0;
	this->charIndex = 0;
}

void DialogueBalloon::hideDialogue()
{
	active = false;
}
