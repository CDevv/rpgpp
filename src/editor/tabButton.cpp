#include "tabButton.hpp"
#include <cstddef>

TabButtonState EditorGuiTabButton(Rectangle rect, std::string text, bool isActive)
{
	Rectangle crossRect = rect;
	crossRect.height = rect.height - 6;
	crossRect.x += (rect.width - rect.height);
	crossRect.y += 3;
	crossRect.width = crossRect.height;

	Rectangle labelRect = rect;
	labelRect.width -= (rect.height + 3);
	labelRect.x += 3;
	labelRect.y += 3;
	labelRect.height -= 6;

	TabButtonState state = TABSTATE_NORMAL;

	//check for state
	if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        state = TABSTATE_FOCUSED;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = TABSTATE_PRESSED;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), crossRect)) {
        state = TABSTATE_X_HOVER;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = TABSTATE_X_PRESSED;
        }
    }

	//tab color
	Color tabColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL));
	if (state == TABSTATE_FOCUSED) {
		tabColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED));
	} else if (state == TABSTATE_PRESSED) {
		tabColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_PRESSED));
	}

	//close button color
	Color crossColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL));
	Color crossTextColor = GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL));
	if (state == TABSTATE_X_HOVER) {
		crossColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED));
		crossTextColor = GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED));
	} else if (state == TABSTATE_X_PRESSED) {
		crossColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_PRESSED));
		crossTextColor = GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED));
	}

	if (!isActive) {
		tabColor.a /= 2;
		crossColor.a /= 2;
	}

	//draw tab button
	DrawRectangleRec(rect, tabColor);
	GuiLabel(labelRect, GuiIconText(ICON_FILETYPE_TEXT, text.c_str()));

	//draw close color
	DrawRectangleRec(crossRect, crossColor);
	GuiDrawIcon(ICON_CROSS, crossRect.x + 1, crossRect.y + 1, 1, crossTextColor);

	return state;
}