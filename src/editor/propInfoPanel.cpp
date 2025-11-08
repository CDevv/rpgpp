#include "propInfoPanel.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "worldViewBox.hpp"

PropInfoPanel::PropInfoPanel() {}

PropInfoPanel::PropInfoPanel(Rectangle rect)
{
    this->rect = rect;
    this->propConcat = "";
    this->currentProp = 0;
}

void PropInfoPanel::setRect(Rectangle rect)
{
    this->rect = rect;
}

void PropInfoPanel::setActionMode(RoomAction action)
{
    this->action = action;
}

int PropInfoPanel::getCurrentPropIndex()
{
    return currentProp;
}

void PropInfoPanel::update()
{

}

void PropInfoPanel::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    std::vector<std::string> vec = fs.getProject()->getTypePaths(FILE_PROP);
    propConcat = "";
    for (auto path : vec) {
        propConcat = propConcat.append(path);
        propConcat.push_back('\0');
    }

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Prop", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

        switch (action) {
            case ACTION_PLACE:

                ImGui::Combo("Prop", &currentProp, propConcat.data());
                break;
            default:
                break;
        }
        ImGui::End();
    }
}
