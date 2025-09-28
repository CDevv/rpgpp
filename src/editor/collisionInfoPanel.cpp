#include "collisionInfoPanel.hpp"
#include <raygui.h>
#include <imgui.h>
#include "worldViewBox.hpp"

CollisionInfoPanel::CollisionInfoPanel() {}

CollisionInfoPanel::CollisionInfoPanel(Rectangle rect)
{
    this->action = ACTION_PLACE;
    this->rect = rect;
}

void CollisionInfoPanel::setRect(Rectangle rect)
{
    this->rect = rect;
}

void CollisionInfoPanel::draw()
{
    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Collisions", nullptr, 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        switch (action) {
            case ACTION_PLACE:
                ImGui::Text("Place a collision tile..");
                break;
            case ACTION_ERASE:
                ImGui::Text("Erase a collision tile..");
                break;
            default:
                break;
        }

        ImGui::End();
    }
}

void CollisionInfoPanel::setActionMode(RoomAction mode)
{
    this->action = mode;
}