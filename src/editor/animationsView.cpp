#include "animationsView.hpp"
#include <raygui.h>
#include "actor.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include <string>

bool TestDropdown(Rectangle rect, std::vector<std::string>* items, int* active, bool* editMode)
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
			Color optionBaseColor = RAYWHITE;
			Color optionBorderColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_NORMAL));

			if (CheckCollisionPointRec(GetMousePosition(), baseRect)) {
				optionBaseColor = GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED));
				optionBorderColor = GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED));

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					*active = index;
					*editMode = false;
				}
			}

			DrawRectangleRec(baseRect, optionBaseColor);
			DrawRectangleLinesEx(baseRect, 1.0f, optionBorderColor);

			float offsetY = (rect.height - 13) / 2;
			DrawTextEx(GuiGetFont(), item.c_str(), Vector2{ baseRect.x + 8, baseRect.y + offsetY }, 13, 2, optionBorderColor);

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

AnimationsView::AnimationsView() {}

AnimationsView::AnimationsView(Rectangle rect)
{
	this->rect = rect;
	this->currentAnim = 0;
	this->animFrames = std::vector<Vector2>{};
	this->animDropdownEditMode = false;
	this->animNames = std::vector<std::string>{};

	animNames.push_back("Down Idle");
	animNames.push_back("Down");
	animNames.push_back("Up Idle");
	animNames.push_back("Up");
	animNames.push_back("Left Idle");
	animNames.push_back("Left");
	animNames.push_back("Right Idle");
	animNames.push_back("Right");
}

void AnimationsView::update()
{
	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	if (actor != nullptr)
	{
		animFrames = actor->getAnimationRaw(static_cast<Direction>(currentAnim));
	}
}

void AnimationsView::draw()
{
	GuiPanel(rect, "Animations View");

	Rectangle baseFrameRect = Rectangle {
		rect.x + 2, rect.y + 58, 48, 48
	};

	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	if (actor != nullptr) {
		Texture texture = actor->getTileSet().getTexture();
		Vector2 tileSize = actor->getTileSet().getTileSize();
		Vector2 origin = Vector2 { 0, 0 };
		float rotation = 0.0f;

		for (Vector2 frameVec : animFrames) {
			Rectangle source = Rectangle {
				tileSize.x * frameVec.x, tileSize.y * frameVec.y,
				tileSize.x, tileSize.y
			};
			DrawTexturePro(texture, source, baseFrameRect, origin, rotation, WHITE);

			DrawRectangleLinesEx(baseFrameRect, 1.0f, GRAY);
			baseFrameRect.x += baseFrameRect.width;
		}
	}

	/*
	std::string dropdownString = "Down Idle;Down;Up Idle;Up;Left Idle;Left;Right Idle;Right";
	if (GuiDropdownBox(Rectangle { rect.x + 2, rect.y + 28, 125, 24 }, dropdownString.c_str(), &currentAnim, animDropdownEditMode)) {
		animDropdownEditMode = !animDropdownEditMode;
	}
	*/

	TestDropdown(Rectangle { rect.x + 2, rect.y + 28, 125, 24 }, &animNames, &currentAnim, &animDropdownEditMode);
}