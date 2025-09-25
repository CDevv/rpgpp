#include "tabList.hpp"
#include "tabButton.hpp"
#include <raymath.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

TabList::TabList() {}

TabList::TabList(Rectangle rect)
{
	this->rect = rect;
	this->scissorRect = rect;
	this->scissorOffset = Vector2 { 0, 0 };
	this->activeIndex = 0;
	this->tabWidth = 120.0f;
	this->drawOverflow = false;

	addItem("Test.");
	addItem("Second.");
	addItem("Third.");
}

void TabList::setRect(Rectangle rect)
{
	this->rect = rect;
}

void TabList::addItem(std::string title)
{
	bool exists = false;
	for (std::vector<TabData>::iterator i = tabs.begin(); i != tabs.end(); ++i)
	{
		if (i->title == title) {
			exists = true;
			break;
		}
	}

	if (exists) return;

	this->tabs.push_back(TabData {title});
	activeIndex = tabs.size() - 1;
}

void TabList::update()
{
	if ((tabs.size() * tabWidth) > rect.width) {
		drawOverflow = true;
		scissorRect = rect;
		scissorRect.width -= (rect.height * 2);
		maxScissorOffset = (tabs.size() * tabWidth) - scissorRect.width;
	} else {
		drawOverflow = false;
		scissorRect = rect;
		scissorOffset = Vector2 { 0, 0 };
	}
}

int TabList::drawTabButton(float offset, std::string title, bool active)
{
	int result = 0;
	float tabTextWidth = ImGui::CalcTextSize(title.c_str()).x; 
	float closeButtonWidth = (rect.height - 4);

	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 buttonColor = style.Colors[ImGuiCol_Button];
    ImVec4 closeColor = ImVec4 { 0, 0, 0, 0 };

    if (active) {
    	buttonColor = style.Colors[ImGuiCol_ButtonActive];
    }

    ImVec2 buttonMin = ImVec2 { rect.x + 4 + offset, rect.y };
    ImVec2 buttonMax = ImVec2 { rect.x + 4 + offset + tabTextWidth + closeButtonWidth + 4, 
    	rect.y + rect.height };

    ImVec2 closeMin = ImVec2 { rect.x + offset + 4 + tabTextWidth, rect.y + 2};
	ImVec2 closeMax = ImVec2 { rect.x + offset + 4 + tabTextWidth + closeButtonWidth, 
		rect.y + 2 + closeButtonWidth };

	//interaction check
    if (ImGui::IsMouseHoveringRect(buttonMin, 
    	buttonMax, false)) {
    	buttonColor = style.Colors[ImGuiCol_ButtonHovered];

    	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    		result = 1;
    	}
    }

    if (ImGui::IsMouseHoveringRect(closeMin, closeMax, false)) {
    	buttonColor = style.Colors[ImGuiCol_Button];
    	if (active) {
	    	buttonColor = style.Colors[ImGuiCol_ButtonActive];
	    }

	    closeColor = style.Colors[ImGuiCol_ButtonHovered];

	    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    		result = 2;
    	}
    }

    //button itself
	draw->AddRectFilled(buttonMin, buttonMax, ImGui::ColorConvertFloat4ToU32(buttonColor));

	//close button
	draw->AddRectFilled(closeMin, closeMax,
		ImGui::ColorConvertFloat4ToU32(closeColor));

	draw->AddLine(ImVec2 { closeMin.x + 2, closeMin.y + 2 }, 
		ImVec2 { closeMin.x + (closeButtonWidth - 2), closeMin.y + (closeButtonWidth - 2) }, 
		IM_COL32_WHITE);
	
	draw->AddLine(ImVec2 { closeMin.x + (closeButtonWidth - 2), closeMin.y + 1 }, 
		ImVec2 { closeMin.x + 2, closeMin.y + (closeButtonWidth - 2) - 1 }, 
		IM_COL32_WHITE);

	//tab label
    draw->AddText(ImVec2 { rect.x + 4 + offset + 4, rect.y + 2 }, IM_COL32_WHITE, title.c_str());

    return result;
}

void TabList::draw()
{
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 buttonColor = style.Colors[ImGuiCol_Button];
    ImVec4 activeColor = style.Colors[ImGuiCol_ButtonActive];

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    int i = 0;
    float addedWidth = 0.0f;
    float closeButtonWidth = (rect.height - 4);

    for (TabData item : tabs) {
    	float tabTextWidth = ImGui::CalcTextSize(item.title.c_str()).x;

    	bool isActive = false;
    	if (activeIndex == i) {
    		isActive = true;
    	}

    	int result = drawTabButton(addedWidth, item.title, isActive);

    	if (result == 1) {
    		activeIndex = i;
    	} else if (result == 2) {
    		tabs.erase(tabs.begin() + i);

    		if (activeIndex == i) {
    			activeIndex = 0;
    		}
    	}

    	addedWidth += tabTextWidth + 8 + closeButtonWidth;
    	i++;
    }

    draw->AddLine(ImVec2 { rect.x, rect.y + rect.height }, 
    	ImVec2 { rect.x + rect.width, rect.y + rect.height }, ImGui::ColorConvertFloat4ToU32(activeColor));
}