#include "mapToolsBox.hpp"
#include <raygui.h>

MapToolsBox::MapToolsBox() {}

MapToolsBox::MapToolsBox(Rectangle rect)
{
	this->rect = rect;
	this->action = ACTION_PLACE;
	this->actionInt = 0;
}

RoomAction MapToolsBox::getActionMode()
{
	return this->action;
}

void MapToolsBox::update()
{
	action = static_cast<RoomAction>(actionInt + 1);
}

void MapToolsBox::draw()
{
	DrawRectangleLinesEx(rect, 1.0f, GRAY);

	Rectangle optionRect = Rectangle { rect.x + 2, rect.y + 2, rect.height - 4, rect.height - 4 };
	GuiToggleGroup(optionRect, "#23#;#28#;#190#", &actionInt);
}