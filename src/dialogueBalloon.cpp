#include "dialogueBalloon.hpp"

#include <raylib.h>
#include <raymath.h>

#include <nlohmann/json.hpp>

#include "game.hpp"
#include "interfaceService.hpp"

DialogueBalloon::DialogueBalloon() = default;

DialogueBalloon::DialogueBalloon(Rectangle rect) {
	this->font = Game::getResources().getFont("LanaPixel");
	this->rect = rect;
	this->textRect = Rectangle{rect.x + 9, rect.y + 9, rect.width - (9 * 2), rect.height - (9 * 2)};
	this->textPortraitRect = this->textRect;
	this->textPortraitRect.width -= this->textRect.height;
	this->textPortraitRect.x += this->textRect.height;
	this->optionsRect = {rect.x + (rect.width - 180), rect.y - 8 - 180, 180, 180};

	this->lineIndex = 0;
	this->sectionIndex = 0;

	this->firstCharTyped = false;
	this->active = false;
	this->frameCounter = 0;
	this->charIndex = 0;

	this->textColor = WHITE;
	this->text =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec "
		"placerat vel nulla eget ullamcorper. Proin varius erat in "
		"tristique dignissim. Aliquam erat volutpat.";
	this->sectionText = {"", text};
	this->lastSectionLen = 0;

	this->textPos = Vector2{0, 0};
}

void DialogueBalloon::update() {
	if (active) {
		if (firstCharTyped == false) {
			firstCharTyped = true;
			auto &soundId = dialogue.lines.at(lineIndex).sections.at(sectionIndex).sound;
			Game::getSounds().playSound(soundId);
			return;
		}

		bool finished = false;
		if (charIndex == (text.size() - 1) || charIndex == text.size()) {
			finished = true;
		}

		this->finishedTyping = finished;

		if (IsKeyPressed(KEY_Z)) {
			if (finished) {
				if (dialogue.lines.at(lineIndex).hasOptions) {
					std::string newDialogue = dialogue.lines.at(lineIndex).options.at(hoveredOption).nextDialogue;
					hideDialogue();
					showDialogue(Game::getBin().dialogues[newDialogue]);
					return;
				}

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

					textPos = Vector2{0, 0};

					appliedTag = false;
					padding = 0.0f;
					maxLineHeight = 0.0f;
				}
			} else {
				charIndex = (text.size() - 1);
			}
		}

		if (dialogue.lines.at(lineIndex).hasOptions) {
			if (IsKeyPressed(KEY_UP) && hoveredOption != 0) {
				hoveredOption--;
			}
			if (IsKeyPressed(KEY_DOWN) && hoveredOption < (dialogue.lines.at(lineIndex).options.size() - 1)) {
				hoveredOption++;
			}
		}

		if (delay) {
			delayDuration -= (GetFrameTime() * static_cast<float>(60.0f / 20.0f));
			if (delayDuration <= 0.0f) {
				delay = false;
			}
		}

		frameCounter++;
		if (frameCounter > (60 / 20) && !delay) {
			frameCounter = 0;
			if (charIndex < text.size()) {
				charIndex++;

				// play sound
				if (charIndex < text.size()) {
					if (text.at(charIndex) != ' ') {
						auto &soundId = dialogue.lines.at(lineIndex).sections.at(sectionIndex).sound;
						Game::getSounds().playSound(soundId);
					}
				}
			}
		}
	}
}

void DialogueBalloon::drawPortrait() const {
	Texture2D texture = Game::getResources().getTexture(dialogue.lines.at(lineIndex).imageId);
	Rectangle source = Rectangle{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
	Rectangle dest = Rectangle{rect.x + 9, rect.y + 9, rect.height - (9 * 2), rect.height - (9 * 2)};
	DrawTexturePro(texture, source, dest, Vector2{0, 0}, 0.0f, WHITE);
}

void DialogueBalloon::draw() {
	if (active) {
		Texture uiTexture = Game::getUi().getTexture();

		Vector2 origin = {0.0f, 0.0f};
		NPatchInfo info =
			NPatchInfo{Rectangle{0, 0, static_cast<float>(uiTexture.width), static_cast<float>(uiTexture.height)},
					   3 * 3, 3 * 3, uiTexture.width - (3 * 3), uiTexture.height - (3 * 3)};

		DrawTextureNPatch(uiTexture, info, rect, origin, 0.0f, WHITE);

		if (finishedTyping && dialogue.lines.at(lineIndex).hasOptions) {
			auto &line = dialogue.lines.at(lineIndex);

			DrawTextureNPatch(uiTexture, info, optionsRect, origin, 0.0f, WHITE);

			Vector2 optionTextPos = {optionsRect.x + 20, optionsRect.y + 20};
			int i = 0;

			for (auto &option : line.options) {
				optionTextPos.y += (13 * 3) * i + (4 * i);

				Color optionTextColor = WHITE;

				if (hoveredOption == i) optionTextColor = YELLOW;

				DrawTextEx(Game::getResources().getFont("LanaPixel"), option.title.c_str(), optionTextPos, 13 * 3, 1.0,
						   optionTextColor);

				i++;
			}
		}

		if (dialogue.lines.at(lineIndex).hasPortrait) {
			drawPortrait();
		}

		textPos = Vector2{0, 0};
		sectionIndex = 0;
		Vector2 charMeasure = Vector2{0, 0};
		for (int i = 0; i < charIndex; i++) {
			int size = 0;
			int idx = 0;
			for (auto section : dialogue.lines.at(lineIndex).sections) {
				if (i < (size + TextLength(section.text.c_str()))) {
					if (sectionIndex != idx) {
						if (section.paddingMode == PADDING_PX) {
							padding = section.padding;
						} else {
							padding = textRect.width * (section.padding / 100);
						}
						appliedTag = false;
						if (section.newline) {
							printf("newline tag.. \n");
						}
					}
					sectionIndex = idx;

					break;
				} else {
					size += TextLength(section.text.c_str());
				}
				idx++;
			}

			if (maxLineHeight < charMeasure.y) {
				maxLineHeight = charMeasure.y;
			}

			auto &line = dialogue.lines.at(lineIndex);
			auto &section = dialogue.lines.at(lineIndex).sections.at(sectionIndex);

			if (fontName != section.font) {
				font = Game::getResources().getFont(section.font);
				fontName = section.font;
			}

			if (section.key == "delay" || section.delay > 0) {
				if (!delay) {
					delay = true;
					delayDuration = section.delay;
				}
			}

			if (!appliedTag) {
				if (section.padding > 0.0f) {
					charMeasure.x += padding;
					appliedTag = true;
				}
			}

			const char *subText = TextSubtext(text.c_str(), i, 1);
			charP(charMeasure, subText, line, section);

			Vector2 newMeasure = MeasureTextEx(Game::getResources().getFont(section.font),
											   TextSubtext(text.c_str(), i, 1), section.textSize * 3, 1.0f);
			charMeasure = newMeasure;
		}
	}
}

void DialogueBalloon::charP(Vector2 charMeasure, const char *c, DialogueLine &textLine,
							DialogueTextSection &textSection) {
	Rectangle resRect = this->textRect;
	if (textLine.hasPortrait) {
		resRect = textPortraitRect;
	}

	Vector2 a = Vector2Add(textPos, Vector2{charMeasure.x + 1, 0.0f});
	Vector2 finalCharPos = Vector2Add(Vector2{resRect.x, resRect.y}, a);

	auto &section = dialogue.lines.at(lineIndex).sections.at(sectionIndex);

	bool hasNewline = false;
	if (!appliedTag && section.newline) {
		hasNewline = true;
	}
	if (TextIsEqual(c, "\n")) {
		hasNewline = true;
	}

	// Check for text overflow on x axis or for newline.
	if ((finalCharPos.x + charMeasure.x) > textRect.width || hasNewline) {
		textPos.x = 0;
		textPos.y += charMeasure.y;

		a = textPos;
		finalCharPos = Vector2Add(Vector2{resRect.x, resRect.y}, a);
	} else {
		textPos.x += charMeasure.x;
	}

	DrawTextEx(Game::getResources().getFont(textSection.font), c, finalCharPos, textSection.textSize * 3, 1,
			   textSection.textColor);
}

void DialogueBalloon::showDialogue(const DialogueBin &newDialogue) {
	if (active) return;

	this->dialogue = newDialogue;

	this->lineIndex = 0;

	firstCharTyped = false;
	active = true;
	text = "";

	for (auto k : dialogue.lines.at(lineIndex).sections) {
		text = text.append(k.text);
	}

	this->frameCounter = 0;
	this->charIndex = 0;
}

void DialogueBalloon::hideDialogue() { active = false; }
