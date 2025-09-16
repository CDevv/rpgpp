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
	this->content = "";
	this->linesCount = 0;
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

		content = content.append(charStr);
		printf("string: %s \n", content.c_str());
	}

	if (IsKeyPressed(KEY_ENTER)) {
		printf("enter \n");

		content = content.append("\n");
		linesCount++;

		cursorPos.x = 2;
		cursorPos.y += static_cast<float>(GuiGetFont().baseSize * 2);

		//this->viewRec.height += (GuiGetFont().baseSize * 2) + 2;
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

	Rectangle contentRec = rect;
    contentRec.height += 100;
    contentRec.width -= 16;
    GuiScrollPanel(rect, NULL, contentRec, &scrollVec, &viewRec);

    BeginScissorMode(viewRec.x, viewRec.y, viewRec.width, viewRec.height);

    Vector2 textPos = Vector2{ rect.x + scrollVec.x + 2, rect.y + scrollVec.y + 2 };
	DrawTextEx(GuiGetFont(), content.c_str(), textPos, GuiGetFont().baseSize * 2, 1, borderColor);

	TextSplit(content.c_str(), '\n', &linesCount);

	/*
	Vector2 textMeasure = MeasureTextEx(GuiGetFont(), content.c_str(), static_cast<float>(GuiGetFont().baseSize * 2), 1.0f);
	Rectangle cursorRect = Rectangle {
		rect.x + scrollVec.x + 2 + textMeasure.x,
		rect.y + scrollVec.y + 2 + ((linesCount - 1) * textMeasure.y),
		4,
		static_cast<float>(GuiGetFont().baseSize * 2),
	};
	*/
	Rectangle cursorRect = Rectangle {
		rect.x + scrollVec.x + cursorPos.x, rect.y + scrollVec.y + cursorPos.y,
		4,
		static_cast<float>(GuiGetFont().baseSize * 2),
	};

	if (state == TEXTAREA_FOCUS) {
		DrawRectangleRec(cursorRect, borderColor);
	}

	EndScissorMode();
}