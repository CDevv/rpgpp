#include "resourceViewerBox.hpp"
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <IconsKenney.h>
#include <string>
#include <vector>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "windowContainer.hpp"
#include "projectFile.hpp"
#include "worldViewBox.hpp"

std::array<EngineFileType, 3> noFuncTypes = { FILE_FONT, FILE_IMAGE, FILE_SOUND };

ResourceViewerBox::ResourceViewerBox() {}

ResourceViewerBox::ResourceViewerBox(Rectangle rect)
{
    this->rect = rect;
    this->dropdownActive = 0;
    this->deleteConfirmOpen = false;
    this->deleteConfirmPath = "";
}

void ResourceViewerBox::setRect(Rectangle rect)
{
    this->rect = rect;
}

void ResourceViewerBox::onNewButton()
{
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();
    EngineFileType fileTypeActive = static_cast<EngineFileType>(dropdownActive);

    if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
        switch (fileTypeActive) {
        default:
            break;
        case FILE_TILESET:
            windows.open("TileSetInit");
            break;
        case FILE_ROOM:
            windows.open("MapInit");
            break;
        case FILE_ACTOR:
            windows.open("ActorInit");
            break;
        case FILE_DIALOGUE:
            windows.open("DialogueInit");
            break;
        }
    }
}

void ResourceViewerBox::update() {}

void ResourceViewerBox::drawResourcesList()
{
    EngineFileType fileTypeActive = static_cast<EngineFileType>(dropdownActive);

    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    std::vector<std::string> paths = fs.getProject()->getTypePaths(fileTypeActive);
    for (std::string path : paths) {
        std::string fileName = GetFileNameWithoutExt(path.c_str());
        if (ImGui::Button(fileName.c_str(), ImVec2 { ImGui::GetWindowWidth(), 24.0f })) {
            bool noFuncType = false;
            for (EngineFileType arrType : noFuncTypes) {
                if (arrType == fileTypeActive) {
                    noFuncType = true;
                }
            }

            if (!noFuncType) {
                fs.openProjectFile(path);
                ui.setInitial();
            }
        }

        drawContextMenu(path);
        windows.drawWindow("DeleteConfirm");
    }
}

void ResourceViewerBox::draw()
{
    const ImVec2 resourceButtonSize = ImVec2 { 76, 26 };

    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    bool gotError = false;

    if (fs.getProject() != nullptr) {
        auto arr = ProjectFile::getTypeNames();

        ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
        ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
        if (ImGui::Begin("Project", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("Resources");
            std::string buttonText = TextFormat("%s %s", ICON_KI_CARET_BOTTOM, arr[dropdownActive].c_str());
            if (ImGui::Button(buttonText.c_str(), ImVec2 { rect.width - (8 * 2), 24 })) {
                ImGui::OpenPopup("res_choose");
            }
            if (ImGui::Button("Refresh", ImVec2 {52.0f, 24.0f})) {
                fs.getProject()->reloadPaths();
            }
            ImGui::SameLine();
            if (ImGui::Button("New..", ImVec2 { rect.width - (8 * 3) - 52, 24.0f })) {
                onNewButton();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 38, 38, 38, 0 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(53,53,53, 255));
            if (ImGui::BeginChild("res_list")) {
                try {
                    drawResourcesList();
                }
                catch (nlohmann::json::exception& ex) {
                    gotError = true;
                    windows.open("Error").setProp(ex.what());
                }

                if (gotError) {
                    windows.open("Error");
                    gotError = false;
                }

                windows.drawWindow("Error");

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
                    if ((i + 1) % 3 != 0) {
                        ImGui::SameLine();
                    }
                }

                ImGui::EndPopup();
            }

            windows.draw();

            if (deleteConfirmOpen) {
                windows.open("DeleteConfirm").setProp(deleteConfirmPath);
                deleteConfirmOpen = false;
            }

            windows.drawWindow("DeleteConfirm");

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
