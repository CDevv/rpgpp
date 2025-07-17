#include "interactableInfoPanel.hpp"
#include <raygui.h>
#include <raylib.h>
#include "worldViewBox.hpp"

InteractableInfoPanel::InteractableInfoPanel() {}

InteractableInfoPanel::InteractableInfoPanel(Rectangle rect)
{
    this->action = ACTION_PLACE;
    this->rect = rect;
    this->type = INT_BLANK;
    this->typeNumber = 0;
    this->typeDropdownEditMode = false;
}

void InteractableInfoPanel::update()
{
    type = static_cast<InteractableType>(typeNumber);
}

void InteractableInfoPanel::draw()
{
    GuiPanel(rect, "Interactable");

    Rectangle labelRect = Rectangle
    {
        rect.x + 8, rect.y + 4 + 24,
        rect.width - 16, (24 * 2)
    };
    Rectangle dropdownRect = labelRect;
    dropdownRect.height = 24;
    dropdownRect.y += 2*24;

    switch (action) {
        case ACTION_PLACE:
            GuiLabel(labelRect, "Place an interactable\nChoose Interactable Type:");
            if (GuiDropdownBox(dropdownRect, "Blank;Two", &typeNumber, typeDropdownEditMode)) {
                typeDropdownEditMode = !typeDropdownEditMode;
            }
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

InteractableType InteractableInfoPanel::getType()
{
    return this->type;
}