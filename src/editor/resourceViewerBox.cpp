#include "resourceViewerBox.hpp"
#include <cstdio>
#include <exception>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <IconsKenney.h>
#include <string>
#include <vector>
#include "windows/deleteConfirmWindow.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "windowContainer.hpp"
#include "projectFile.hpp"
#include "worldViewBox.hpp"

ResourceViewerBox::ResourceViewerBox() {}

ResourceViewerBox::ResourceViewerBox(Rectangle rect)
{
    this->rect = rect;
    this->dropdownEditMode = false;
    this->dropdownActive = 0;
    this->deleteConfirmOpen = false;
    this->deleteConfirmPath = "";

    this->errorMessage = "";
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
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    bool gotError = false;

    if (fs.getProject() != nullptr) {
        auto arr = ProjectFile::getTypeNames();
        EngineFileType fileTypeActive = static_cast<EngineFileType>(dropdownActive);

        ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
        ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
        if (ImGui::Begin("Project", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("Resources");
            std::string buttonText = TextFormat("%s %s", ICON_KI_CARET_BOTTOM, arr[dropdownActive].c_str());
            if (ImGui::Button(buttonText.c_str(), ImVec2 { rect.width - (8 * 2), 24 })) {
                ImGui::OpenPopup("res_choose");
            }
            if (ImGui::Button("New..", ImVec2 { rect.width - (8 * 2), 24.0f })) {
                if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
                    switch (fileTypeActive) {
                    default:
                        break;
                    case FILE_TILESET:
                        windows.openTileSetInit();
                        break;
                    case FILE_ROOM:
                        windows.openMapInit();
                        break;
                    case FILE_ACTOR:
                        windows.openActorInit();
                        break;
                    }
                }
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 38, 38, 38, 0 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(53,53,53, 255));
            if (ImGui::BeginChild("res_list")) {
                try {
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
                }
                catch (nlohmann::json::out_of_range& ex) {
                    gotError = true;
                    errorMessage = std::string(ex.what());
                }
                windows.drawDeleteConfirm();

                if (gotError) {
                    ImGui::OpenPopup("Runtime Error");
                    gotError = false;
                }

                ImGui::SetNextWindowSize(ImVec2 { 240, -1 });

                if (ImGui::BeginPopupModal("Runtime Error")) {
                    ImGui::TextWrapped("%s", errorMessage.c_str());
                    if (ImGui::Button("Close")) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::SetNextWindowPos(ImVec2 { rect.x + 8, rect.y + 70 });
            if (ImGui::BeginPopup("res_choose")) {
                for (int i = 0; i < arr.size(); i++) {
                    if (ImGui::Button(arr[i].c_str(), resourceButtonSize)) {
                        dropdownActive = i;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                }

                ImGui::EndPopup();
            }

            windows.draw();

            if (deleteConfirmOpen) {
                DeleteConfirmWindow& deleteConfirm = windows.openDeleteConfirm();
                deleteConfirm.setFilePath(deleteConfirmPath);
                deleteConfirmOpen = false;
            }

            windows.drawDeleteConfirm();

            ImGui::End();
        }
    }
}

void ResourceViewerBox::drawContextMenu(std::string resPath)
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();
    std::string resFileName = GetFileNameWithoutExt(resPath.c_str());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
    if (ImGui::BeginPopupContextItem()) {
        ImGui::Text("%s", resFileName.c_str());
        ImGui::Separator();

        std::string fullPath = TextFormat("%s/%s", fs.getProject()->getProjectBasePath().c_str(),
            resPath.c_str());
        if (ImGui::MenuItem("Copy relative path")) {
            SetClipboardText(resPath.c_str());
        }
        if (ImGui::MenuItem("Copy full path")) {
            SetClipboardText(fullPath.c_str());
        }
        if (ImGui::MenuItem("Delete")) {
            deleteConfirmPath = fullPath;
            deleteConfirmOpen = true;
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void ResourceViewerBox::drawTileSets()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    std::vector<std::string> tileSetPaths = fs.getProject()->getTileSetPaths();
    for (std::string tileSetPath : tileSetPaths) {
        std::string tileSetFileName = GetFileNameWithoutExt(tileSetPath.c_str());
        if (ImGui::Button(tileSetFileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f })) {
            fs.openProjectFile(tileSetPath);
            ui.setInitial();
        }

        drawContextMenu(tileSetPath);
        windows.drawDeleteConfirm();
    }
}

void ResourceViewerBox::drawMaps()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    std::vector<std::string> mapPaths = fs.getProject()->getMapPaths();

    for (std::string mapPath : mapPaths) {
        std::string mapFileName = GetFileNameWithoutExt(mapPath.c_str());
        if (ImGui::Button(mapFileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f })) {
            fs.openProjectFile(mapPath);
            ui.setInitial();
        }

        drawContextMenu(mapPath);
    }
}

void ResourceViewerBox::drawActors()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    std::vector<std::string> actorPaths = fs.getProject()->getActorPaths();
    for (std::string actorPath : actorPaths) {
        std::string actorFileName = GetFileNameWithoutExt(actorPath.c_str());
        if (ImGui::Button(actorFileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f })) {
            fs.openProjectFile(actorPath);
            ui.setInitial();
        }

        drawContextMenu(actorPath);
    }
}
