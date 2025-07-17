#include "interactableInfoPanel.hpp"
#include <raygui.h>
#include <raymath.h>
#include <raylib.h>
#include "worldViewBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"

InteractableInfoPanel::InteractableInfoPanel() {}

InteractableInfoPanel::InteractableInfoPanel(Rectangle rect)
{
    this->action = ACTION_PLACE;
    this->rect = rect;
    this->lastType = INT_BLANK;
    this->type = INT_BLANK;
    this->typeNumber = 0;
    this->typeDropdownEditMode = false;
    this->interactableWorldPos = Vector2 { -1, -1 };
}

void InteractableInfoPanel::setInitial(InteractableType type)
{
    this->type = type;
}

void InteractableInfoPanel::update()
{
    type = static_cast<InteractableType>(typeNumber);

    FileSystemService& fs = Editor::getFileSystem();
    
    if (lastType != type) {
        fs.getRoom()->getInteractables().setInteractableType(interactableWorldPos.x, interactableWorldPos.y, type);
        lastType = type;
    }
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
        case ACTION_EDIT:
            if (interactableWorldPos.x > -1) {
                GuiLabel(labelRect, "Edit the selected interactable\nChoose Interactable Type:");
                if (GuiDropdownBox(dropdownRect, "Blank;Two", &typeNumber, typeDropdownEditMode)) {
                    typeDropdownEditMode = !typeDropdownEditMode;
                }
            } else {
                GuiLabel(labelRect, "No selected interactable..");
            }
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

void InteractableInfoPanel::setSelectedInteractable(Interactable interactable)
{
    if (interactable.isValid()) {
        if (!Vector2Equals(interactableWorldPos, interactable.getWorldPos())) {
            this->typeNumber = static_cast<int>(interactable.getType());
        }

        interactableWorldPos = interactable.getWorldPos();
    } else {
        interactableWorldPos = Vector2 { -1, -1 };
    }
}