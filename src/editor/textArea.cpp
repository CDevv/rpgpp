#include "textArea.hpp"
#include <raygui.h>

TextArea::TextArea() {}

TextArea::TextArea(Rectangle rect)
{
	this->state = TEXTAREA_NORMAL;
	this->rect = rect;
	this->viewRec = Rectangle {0 };
	this->scrollVec = Vector2 { 0, 0 };
	this->cursorPos = Vector2 { 2, 2 };
	this->cursorIndex = 0;
	this->content = "";
	this->linesCount = 0;
	this->lineStartIndex = 0;
	this->lineEndIndex = 0;

	this->contentRec = rect;
	contentRec.width -= 16;
}

void TextArea::update()
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(GetMousePosition(), rect)) {
			state = TEXTAREA_FOCUS;
		} else {
			state = TEXTAREA_NORMAL;
		}
	} else {
		if (state != TEXTAREA_FOCUS) {
			if (CheckCollisionPointRec(GetMousePosition(), rect)) {
				state = TEXTAREA_HOVER;
			} else {
				state = TEXTAREA_NORMAL;
			}
		}
	}

	if (state == TEXTAREA_FOCUS) {
		keyInput();
	}
}

void TextArea::keyInput()
{
	int unicode = GetCharPressed();
		
	if (unicode != 0) {
		printf("unicode: %i \n", unicode);
		int count = 0;
		std::string charStr = CodepointToUTF8(unicode, &count);

		Vector2 textMeasure = MeasureTextEx(GuiGetFont(), charStr.c_str(), static_cast<float>(GuiGetFont().baseSize * 2), 1.0f);
		cursorPos.x += textMeasure.x + 1;

		//content = content.append(charStr);

		content = content.insert(cursorIndex, charStr);
		cursorIndex++;

		printf("string: %s \n", content.c_str());
	}

	if (IsKeyPressed(KEY_ENTER)) {
		printf("enter \n");

		content = content.insert(cursorIndex, "\n");
		linesCount++;
		cursorIndex++;
		lineStartIndex = cursorIndex;

		cursorPos.x = 2;
		cursorPos.y += static_cast<float>(GuiGetFont().baseSize * 2);

		contentRec.height += static_cast<float>(GuiGetFont().baseSize * 2);
	}

	if (IsKeyPressed(KEY_LEFT)) {
		if (cursorIndex > 0) {
			std::string charStr = "";
			cursorIndex--;

			if (cursorIndex != 0) {
				std::string sub = "";
				if (cursorIndex == content.size()) {
					sub = TextSubtext(content.c_str(), 0, cursorIndex - 1);	
				} else {
					sub = TextSubtext(content.c_str(), 0, cursorIndex);	
				}		
				Vector2 textMeasure = MeasureTextEx(GuiGetFont(), sub.c_str(), static_cast<float>(GuiGetFont().baseSize * 2), 1.0f);
				cursorPos.x = 2 + textMeasure.x;
			} else {
				cursorPos.x = 2;
			}

			//printf("%c \n", content.at(cursorIndex));
		}
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		if (cursorIndex < (content.size())) {
			cursorIndex++;

			std::string sub = "";
			sub = TextSubtext(content.c_str(), 0, cursorIndex);
			Vector2 textMeasure = MeasureTextEx(GuiGetFont(), sub.c_str(), static_cast<float>(GuiGetFont().baseSize * 2), 1.0f);
			cursorPos.x = 2 + textMeasure.x;

			//printf("%c \n", content.at(cursorIndex));
		}
	}
}

void TextArea::draw()
{
	Color borderColor = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL));

	switch (state) {
	case TEXTAREA_HOVER:
		borderColor = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED));
		break;
	case TEXTAREA_FOCUS:
		borderColor = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_PRESSED));
		break;
	default:
		borderColor = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL));
		break;
	}

	DrawRectangleLinesEx(rect, 1.0f, borderColor);

    GuiScrollPanel(rect, NULL, contentRec, &scrollVec, &viewRec);

    BeginScissorMode(viewRec.x, viewRec.y, viewRec.width, viewRec.height);

    Vector2 textPos = Vector2{ rect.x + scrollVec.x + 2, rect.y + scrollVec.y + 2 };
	DrawTextEx(GuiGetFont(), content.c_str(), textPos, GuiGetFont().baseSize * 2, 1, borderColor);

	TextSplit(content.c_str(), '\n', &linesCount);

	Rectangle cursorRect = Rectangle {
		rect.x + scrollVec.x + cursorPos.x, 
		rect.y + scrollVec.y + cursorPos.y,
		2,
		static_cast<float>(GuiGetFont().baseSize * 2),
	};

	if (state == TEXTAREA_FOCUS) {
		DrawRectangleRec(cursorRect, borderColor);
	}

	EndScissorMode();
}