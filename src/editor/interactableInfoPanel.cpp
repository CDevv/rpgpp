#include "interactableInfoPanel.hpp"
#include <raymath.h>
#include <raylib.h>
#include <imgui.h>
#include <raygui.h>
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
    this->interactable = nullptr;

    this->diagText = std::make_unique<char[]>(1);
    diagText[0] = '\0';
    this->diagTextEditMode = false;
}

void InteractableInfoPanel::setRect(Rectangle rect)
{
    this->rect = rect;
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
        if (interactableWorldPos.x != -1) {
            fs.getRoom()->getInteractables().setInteractableType(interactableWorldPos.x, interactableWorldPos.y, type);
            lastType = type;
        }
    }
}

void InteractableInfoPanel::draw()
{
    /*
    GuiPanel(rect, "Interactable");

    if (typeDropdownEditMode) GuiLock();

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

                if (interactable != nullptr) {
                    drawTypeProps();
                }
            } else {
                GuiLabel(labelRect, "No selected interactable..");
            }
            break;
        default:
            break;
    }
    */

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Interactable", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        switch (action) {
        case ACTION_PLACE:
            ImGui::Text("Place an interactable");
            ImGui::Combo("Type", &typeNumber, "Blank\0Two\0");
            break;
        case ACTION_ERASE:
            ImGui::Text("Erase an interactable..");
            break;
        case ACTION_EDIT:
            ImGui::Text("Edit the selected interactable");
            ImGui::Combo("Type", &typeNumber, "Blank\0Two\0");
            break;
        default:
            break;
        }

        ImGui::End();
    }
}

void InteractableInfoPanel::drawTypeProps()
{
    Rectangle saveRect = Rectangle {
        rect.x + 4, (rect.y + rect.height) - (24 + 4),
        rect.width - 8, 24
    };

    if (GuiButton(saveRect, "Save..")) {
        saveProps();
    }

    switch (interactable->getType()) {
    case INT_TWO:
        drawDialogueProps();
        break;
    default:
        break;
    }
}

void InteractableInfoPanel::drawDialogueProps()
{
    Rectangle baseRect = Rectangle {
        rect.x + 8, rect.y + 4 + (24 * 3),
        rect.width - 16, 24
    };

    Rectangle lineRect = baseRect;
    lineRect.height = 4;
    lineRect.y += 24;
    GuiLine(lineRect, NULL);

    Rectangle labelRect = baseRect;
    labelRect.y += 24 + 4;
    GuiLabel(labelRect, "Dialogue Text");

    Rectangle textRect = baseRect;
    textRect.y += (2 * 24) + 4;
    
    if (GuiTextBox(textRect, diagText.get(), 13, diagTextEditMode)) {
        diagTextEditMode = !diagTextEditMode;
    }
    
    //GuiLabel(textRect, propsState.getDialogue().lines.at(0).text.c_str());
}

void InteractableInfoPanel::saveProps()
{
    Dialogue dialogue;
    InteractableTwo* dialogueInter;

    if (interactable->getType() == INT_TWO) {
        dialogue = propsState.getDialogue();
        dialogue.lines.at(0).text = diagText.get();

        propsState.setDialogue(dialogue);
        dialogueInter = static_cast<InteractableTwo*>(interactable);
        dialogueInter->setDialogue(dialogue);
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

void InteractableInfoPanel::setSelectedInteractable(Interactable* interactable)
{
    if (interactable == nullptr) {
        interactableWorldPos = Vector2 { -1, -1 };
        return;
    };

    if (interactable->isValid()) {
        this->interactable = interactable;

        if (interactable->getType() == INT_TWO) {
            InteractableTwo* diagInt = static_cast<InteractableTwo*>(interactable);
            if (diagInt->getDialogue().lines.size() == 0) {
                Dialogue dialogue;
                dialogue.lines.push_back(DialogueLine {
                    "Empty", "Lorum ipsum!"
                });
                diagInt->setDialogue(dialogue);
            }
        }

        //if its the same one..
        if (!Vector2Equals(interactableWorldPos, interactable->getWorldPos())) {
            this->typeNumber = static_cast<int>(interactable->getType());

            this->propsState.setDefaults(interactable);

            if (interactable->getType() == INT_TWO) {
                std::string diagString = "Lorum ipsum!";
                if (propsState.getDialogue().lines.size() != 0) {
                    diagString = propsState.getDialogue().lines.at(0).text;
                }
                int arrSize = diagString.size();
                this->diagText.reset(new char[arrSize]);
                std::copy(diagString.begin(), diagString.end(), diagText.get());
                this->diagText[arrSize] = '\0';
            }
        }

        interactableWorldPos = interactable->getWorldPos();
    } else {
        interactableWorldPos = Vector2 { -1, -1 };
    }
}