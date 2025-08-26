#include "editorInterfaceService.hpp"
#include <raylib.h>
#include <raygui.h>

bool EditorGuiDropdown(Rectangle rect, std::vector<std::string>* items, int* active, bool* editMode)
{
	bool pressed = false;
	Color baseColor = GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL));
	Color borderColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_NORMAL));

	if (CheckCollisionPointRec(GetMousePosition(), rect))
	{
		baseColor = GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED));
		borderColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED));

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			pressed = true;

			baseColor = GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED));
			borderColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_PRESSED));
		}
	}

	if (pressed) {
		if (items != nullptr) {
			*editMode = !(*editMode);
		}
	}

	if (*editMode) {
		Rectangle baseRect = rect;

		Rectangle menuRect = rect;
		menuRect.height *= items->size();
		menuRect.y -= menuRect.height;
		DrawRectangleRec(menuRect, RAYWHITE);
		DrawRectangleLinesEx(menuRect, 1.0f, DARKGRAY);

		baseRect.y = menuRect.y;
		int index = 0;
		for (std::string item : *items) {
			Color optionBaseColor = BLANK;
			Color optionBorderColor = BLANK;
			Color optionTextColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_NORMAL));

			if (CheckCollisionPointRec(GetMousePosition(), baseRect)) {
				optionBaseColor = GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED));
				optionBorderColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED));
				optionTextColor = optionBorderColor;

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					*active = index;
					*editMode = false;
				}
			}

			DrawRectangleRec(baseRect, optionBaseColor);
			DrawRectangleLinesEx(baseRect, 1.0f, optionBorderColor);

			float offsetY = (rect.height - 13) / 2;
			DrawTextEx(GuiGetFont(), item.c_str(), Vector2{ baseRect.x + 8, baseRect.y + offsetY }, 13, 2, optionTextColor);

			index++;
			baseRect.y += baseRect.height;
		}
	}

	DrawRectangleRec(rect, baseColor);
	DrawRectangleLinesEx(rect, 1.0f, borderColor);
	if (items != NULL) {
		float offsetY = (rect.height - 13) / 2;
		DrawTextEx(GuiGetFont(), items->at(*active).c_str(), Vector2 { rect.x + 8, rect.y + offsetY }, 13, 2, borderColor);
	}

	float iconOffsetY = (rect.height - 16) / 2;
	GuiDrawIcon(ICON_BURGER_MENU, (rect.x + (rect.width - 24)), (rect.y + iconOffsetY), 1, borderColor);

	return pressed;
}