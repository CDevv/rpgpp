#include "actorInfoPanel.hpp"
#include "worldViewBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <imgui.h>

ActorInfoPanel::ActorInfoPanel() {}

ActorInfoPanel::ActorInfoPanel(Rectangle rect)
{
    this->rect = rect;
    this->actorConcat = "";
    this->currentActor = 0;
}

void ActorInfoPanel::setRect(Rectangle rect)
{
    this->rect = rect;
}

void ActorInfoPanel::setActionMode(RoomAction action)
{
    this->action = action;
}

int ActorInfoPanel::getCurrentActorIndex()
{
    return currentActor;
}

void ActorInfoPanel::update()
{

}

void ActorInfoPanel::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    std::vector<std::string> vec = fs.getProject()->getTypePaths(FILE_PROP);
    actorConcat = "";
    for (auto path : vec) {
        actorConcat = actorConcat.append(path);
        actorConcat.push_back('\0');
    }

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Prop", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

        switch (action) {
            case ACTION_PLACE:
                ImGui::Combo("Actor", &currentActor, actorConcat.data());
                break;
            default:
                break;
        }
        ImGui::End();
    }
}
