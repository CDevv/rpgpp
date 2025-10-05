#include "interactableInfoPanel.hpp"
#include <raymath.h>
#include <raylib.h>
#include <imgui.h>
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

    strcpy(diagChars, "Hello.");
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
            if (interactableWorldPos.x != -1) {
                ImGui::Text("Edit the selected interactable");
                ImGui::Combo("Type", &typeNumber, "Blank\0Two\0");

                drawTypeProps();
            }
            break;
        default:
            break;
        }

        ImGui::End();
    }
}

void InteractableInfoPanel::drawTypeProps()
{
    if (ImGui::BeginChild("type_props", ImVec2(0, 0), ImGuiChildFlags_Borders, 0)) {
        switch (interactable->getType()) {
        case INT_TWO:
            drawDialogueProps();
            break;
        default:
            break;
        }

        ImGui::EndChild();
    }
}

void InteractableInfoPanel::drawDialogueProps()
{
    ImGui::InputTextMultiline("Dialogue.", propsState.diagText, IM_ARRAYSIZE(propsState.diagText));
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
