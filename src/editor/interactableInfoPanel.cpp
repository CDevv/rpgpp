#include "interactableInfoPanel.hpp"
#include <raygui.h>
#include <raylib.h>
#include "worldViewBox.hpp"

InteractableInfoPanel::InteractableInfoPanel() {}

InteractableInfoPanel::InteractableInfoPanel(Rectangle rect)
{
    this->action = ACTION_PLACE;
    this->rect = rect;
}

void InteractableInfoPanel::draw()
{
    GuiPanel(rect, "Interactable");

    Rectangle labelRect = Rectangle
    {
        rect.x + 4, rect.y + 4,
        rect.width - 8, rect.height - 8
    };
    switch (action) {
        case ACTION_PLACE:
            GuiLabel(labelRect, "Place an interactable of blank type..");
            break;
        case ACTION_ERASE:
            GuiLabel(labelRect, "Erase an interactable..");
            break;
        default:
            break;
    }
}

void InteractableInfoPanel::setActionMode(RoomAction mode)
{
    this->action = mode;
}