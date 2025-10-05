#include "mapToolsBox.hpp"
#include "IconsKenney.h"
#include <imgui.h>

#include <array>
#include <string>
#include <IconsKenney.h>

static std::array<std::string, 4> buttonIcons = {
	ICON_KI_CURSOR,
	ICON_KI_PENCIL,
	ICON_KI_ERASER,
	ICON_KI_COG
};

MapToolsBox::MapToolsBox() {}

MapToolsBox::MapToolsBox(Rectangle rect)
{
	this->rect = rect;
	this->action = ACTION_PLACE;
	this->actionInt = 0;
}

void MapToolsBox::setRect(Rectangle rect)
{
	this->rect = rect;
}

RoomAction MapToolsBox::getActionMode()
{
	return this->action;
}

void MapToolsBox::update()
{
	action = static_cast<RoomAction>(actionInt);
}

void MapToolsBox::draw()
{
	ImVec2 buttonSize = ImVec2 { rect.height - 8, rect.height - 8 };

	ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
	ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	if (ImGui::Begin("MapToolsBox", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

		for (int i = 0; i < buttonIcons.size(); i++) {
			if (actionInt != i) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 38, 38, 38, 0 });
            	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(53,53,53, 255));
			}

			if (ImGui::Button(buttonIcons[i].c_str(), buttonSize)) {
				ImGui::PopStyleColor(2);
				actionInt = i;
			}

			ImGui::SameLine();

			if (actionInt != i) {
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::End();
	}
	ImGui::PopStyleVar();
}
