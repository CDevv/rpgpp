#include "collisionInfoPanel.hpp"
#include <raygui.h>
#include <raylib.h>
#include "worldViewBox.hpp"

CollisionInfoPanel::CollisionInfoPanel() {}

CollisionInfoPanel::CollisionInfoPanel(Rectangle rect)
{
    this->action = ACTION_PLACE;
    this->rect = rect;
}

void CollisionInfoPanel::draw()
{
    GuiPanel(rect, "Collisions");
    Rectangle labelRect = Rectangle
    {
        rect.x + 4, rect.y + 4,
        rect.width - 8, rect.height - 8
    };
    switch (action) {
        case ACTION_PLACE:
            GuiLabel(labelRect, "Place a collision tile..");
            break;
        case ACTION_ERASE:
            GuiLabel(labelRect, "Erase a collision tile..");
            break;
        default:
            break;
    }
}

void CollisionInfoPanel::setActionMode(RoomAction mode)
{
    this->action = mode;
}