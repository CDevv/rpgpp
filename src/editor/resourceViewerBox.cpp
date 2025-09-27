#include "resourceViewerBox.hpp"
#include <raygui.h>
#include <raylib.h>
#include <string>
#include <vector>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "windowContainer.hpp"
#include "projectFile.hpp"

ResourceViewerBox::ResourceViewerBox() {}

ResourceViewerBox::ResourceViewerBox(Rectangle rect)
{
    this->rect = rect;
    this->dropdownEditMode = false;
    this->dropdownActive = 0;
}

void ResourceViewerBox::setRect(Rectangle rect)
{
    this->rect = rect;
}

void ResourceViewerBox::update() {}

void ResourceViewerBox::draw()
{
    const ImVec2 resourceButtonSize = ImVec2 { 76, 76 };

    FileSystemService& fs = Editor::getFileSystem();
    if (fs.getProject() != nullptr) {
        auto arr = ProjectFile::getTypeNames();
        EngineFileType fileTypeActive = static_cast<EngineFileType>(dropdownActive);

        ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
        ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
        if (ImGui::Begin("Project")) {
            ImGui::Text("Resources");
            if (ImGui::Button(arr[dropdownActive].c_str(), ImVec2 { rect.width - (8 * 2), 24 })) {
                ImGui::OpenPopup("res_choose");
            }
            if (ImGui::Button("New..", ImVec2 { rect.width - (8 * 2), 24.0f })) {
                ImGui::OpenPopup("new_");
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 38, 38, 38, 0 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(53,53,53, 255));
            if (ImGui::BeginChild("res_list")) {


                switch (fileTypeActive) {
                default:
                    break;
                case FILE_TILESET:
                    drawTileSets();
                    break;
                case FILE_ROOM:
                    drawMaps();
                    break;
                case FILE_ACTOR:
                    drawActors();
                    break;
                }

                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::SetNextWindowPos(ImVec2 { rect.x + 8, rect.y + 70 });
            if (ImGui::BeginPopup("res_choose")) {          
                for (int i = 0; i < arr.size(); i++) {
                    ImGui::SameLine();
                    if (ImGui::Button(arr[i].c_str(), resourceButtonSize)) {
                        dropdownActive = i;
                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopupModal("new_")) {
                ImGui::Text("Content.");
                ImGui::EndPopup();
            }

            ImGui::End();
        }
    }
}

void ResourceViewerBox::drawTileSets()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    /*
    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New")) {
        if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
            windows.openTileSetInit();
        }
    }
    */
    

    std::vector<std::string> tileSetPaths = fs.getProject()->getTileSetPaths();
    for (std::string tileSetPath : tileSetPaths) {
        std::string tileSetFileName = GetFileNameWithoutExt(tileSetPath.c_str());
        if (ImGui::Button(tileSetFileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f })) {
            fs.openProjectFile(tileSetPath);
            ui.setInitial();
        }
    }
}

void ResourceViewerBox::drawMaps()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    /*
    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New..")) {
        if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
            windows.openMapInit();
        }
    }
    */
    std::vector<std::string> mapPaths = fs.getProject()->getMapPaths();

    for (std::string mapPath : mapPaths) {
        std::string mapFileName = GetFileNameWithoutExt(mapPath.c_str());
        if (ImGui::Button(mapFileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f })) {
            fs.openProjectFile(mapPath);
            ui.setInitial();
        }
    }
}

void ResourceViewerBox::drawActors()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    /*
    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New..")) {
        if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
            windows.openActorInit();
        }
    }
    */

    std::vector<std::string> actorPaths = fs.getProject()->getActorPaths();
    for (std::string actorPath : actorPaths) {
        std::string actorFileName = GetFileNameWithoutExt(actorPath.c_str());
        ImGui::Button(actorFileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f });

        //fs.openProjectFile(actorPath);
        //ui.setInitial();
    }
}