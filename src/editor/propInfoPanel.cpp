#include "propInfoPanel.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "interactableInfoPanel.hpp"
#include "worldViewBox.hpp"

PropInfoPanel::PropInfoPanel() {}

PropInfoPanel::PropInfoPanel(Rectangle rect)
{
    this->rect = rect;
    this->propConcat = "";
    this->currentProp = 0;
    this->prop = nullptr;
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

void PropInfoPanel::setProp(Prop* p)
{
    this->prop = p;
    if (p != nullptr) {
        if (p->getHasInteractable()) {
            intPropsState.setDefaults(prop->getInteractable());
        }
    }
}

void PropInfoPanel::update()
{
    if (prop != nullptr) {
        intPropsState.updateProps(prop->getInteractable());
    }
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
            case ACTION_EDIT:
                if (prop != nullptr) {
                    ImGui::LabelText("Source", "%s", prop->getSourcePath().c_str());
                    ImGui::Checkbox("Interactable: On Touch", &intPropsState.onTouch);
                    InteractableInfoPanel::drawTypeProps(prop->getInteractable()->type, &intPropsState);
                } else {
                    currentProp = 0;
                }
                break;
            default:
                break;
        }
        ImGui::End();
    }
}
