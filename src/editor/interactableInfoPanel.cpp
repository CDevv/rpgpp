#include "interactableInfoPanel.hpp"
#include <raymath.h>
#include <raylib.h>
#include <imgui.h>
#include "interactable.hpp"
#include "nfd.h"
#include "room.hpp"
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
    this->interBase = nullptr;

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
            Room* room = fs.getCurrentFile()->getData<Room>();
            room->getInteractables().setInteractableType(interactableWorldPos.x, interactableWorldPos.y, type);
            lastType = type;
        }
    }

    if (interactableWorldPos.x != -1 && interBase != nullptr) {
        propsState.updateProps(interBase);
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
            ImGui::Combo("Type", &typeNumber, "Blank\0Dialogue\0Warper\0");
            break;
        case ACTION_ERASE:
            ImGui::Text("Erase an interactable..");
            break;
        case ACTION_EDIT:
            if (interactableWorldPos.x != -1) {
                ImGui::Text("Edit the selected interactable");
                ImGui::Combo("Type", &typeNumber, "Blank\0Dialogue\0Warper\0");
                ImGui::Checkbox("Interact On Touch", &propsState.onTouch);

                int posX = static_cast<int>(interBase->pos.x);
                int posY = static_cast<int>(interBase->pos.y);
                ImGui::Text("Interactable pos: [%i, %i]", posX, posY);

                drawTypeProps(interBase->type, &this->propsState);
            }
            break;
        default:
            break;
        }

        ImGui::End();
    }
}

void InteractableInfoPanel::drawTypeProps(InteractableType intType, InteractablePropsState* propsState)
{
    if (ImGui::BeginChild("type_props", ImVec2(0, 0), ImGuiChildFlags_Borders, 0)) {
        switch (intType) {
        case INT_TWO:
            drawDialogueProps(propsState);
            break;
        case INT_WARPER:
            drawWarperProps(propsState);
            break;
        default:
            break;
        }

        ImGui::EndChild();
    }
}

void InteractableInfoPanel::drawDialogueProps(InteractablePropsState* propsState)
{
    std::string diagSource = propsState->diagSource;
    diagSource.push_back('\0');
    ImGui::InputText("Dialogue.", diagSource.data(), diagSource.size(),
        ImGuiInputTextFlags_ReadOnly);
    if (ImGui::Button("Open..", ImVec2(-1, 16))) {
        FS_Result fsResult = Editor::getFileSystem().openDialogueResource();

        if (fsResult.result == NFD_OKAY) {
            propsState->diagSource = std::string(GetFileNameWithoutExt(fsResult.path.c_str()));
        }
    }
}

void InteractableInfoPanel::drawWarperProps(InteractablePropsState* propsState)
{
    std::string warperTarget = propsState->diagSource;
    warperTarget.push_back('\0');
    ImGui::InputText("Target Room", warperTarget.data(), warperTarget.size(),
        ImGuiInputTextFlags_ReadOnly);
    if (ImGui::Button("Open..", ImVec2(-1, 16))) {
        nfdu8filteritem_t filters[1] = { { "RPG++ Room", "rmap" } };
        FS_Result fsResult = Editor::getFileSystem().openFile(filters);

        if (fsResult.result == NFD_OKAY) {
            propsState->diagSource = std::string(GetFileName(fsResult.path.c_str()));
        }
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

void InteractableInfoPanel::setSelected(IntBaseWrapper* inter)
{
    if (inter == nullptr) {
        interBase = nullptr;
        interactableWorldPos = Vector2 { -1, -1 };
        return;
    }

    interBase = inter;

    if (!Vector2Equals(interactableWorldPos, inter->pos)) {
        this->typeNumber = static_cast<int>(inter->type);
        this->propsState.setDefaults(inter);
    }

    interactableWorldPos = inter->pos;
}
