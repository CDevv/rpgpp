#include "resourceViewerBox.hpp"
#include <raygui.h>
#include <raylib.h>
#include <string>
#include <vector>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "windowContainer.hpp"

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
    if (dropdownEditMode) GuiLock();

    /*
    FileSystemService& fs = Editor::getFileSystem();
    if (fs.getProject() != nullptr) {
        GuiPanel(rect, "Project");

        EngineFileType fileTypeActive = static_cast<EngineFileType>(dropdownActive);

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

        if (GuiDropdownBox(Rectangle { rect.x, rect.y + 24, rect.width, 24 }, "TileSets;Rooms;Actors", &dropdownActive, dropdownEditMode)) {
            dropdownEditMode = !dropdownEditMode;
        }
    }
    */

    const ImVec2 resourceButtonSize = ImVec2 { 76, 76 };

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("Project")) {
        ImGui::Text("Resources");
        if (ImGui::Button("Choose..", ImVec2 { rect.width - (8 * 2), 24 })) {
            ImGui::OpenPopup("res_choose");
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 38, 38, 38, 0 });
        //ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 56, 72, 78, 0 });
        if (ImGui::BeginChild("res_list")) {
            for (int x = 0; x < 100; x++) {
                ImGui::Button(TextFormat("Item-%i", x), ImVec2 { ImGui::GetWindowWidth(), 16.0f });
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        //ImGui::PopStyleColor();

        ImGui::SetNextWindowPos(ImVec2 { rect.x + 8, rect.y + 70 });
        if (ImGui::BeginPopup("res_choose")) {
            ImGui::Button("Example", resourceButtonSize);
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}

void ResourceViewerBox::drawTileSets()
{
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New")) {
        if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
            windows.openTileSetInit();
        }
    }

    Rectangle baseRect = Rectangle { rect.x, rect.y, rect.width, rect.height };
    baseRect.y += 3*24;
    baseRect.height = 24;

    FileSystemService& fs = Editor::getFileSystem();

    std::vector<std::string> tileSetPaths = fs.getProject()->getTileSetPaths();
    for (std::string tileSetPath : tileSetPaths) {
        std::string tileSetFileName = GetFileNameWithoutExt(tileSetPath.c_str());
        if (GuiLabelButton(baseRect, tileSetFileName.c_str())) {
            if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
                fs.openProjectFile(tileSetPath);
                ui.setInitial();
            }
        }
        baseRect.y += 24;
    }
}

void ResourceViewerBox::drawMaps()
{
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New..")) {
        if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
            windows.openMapInit();
        }
    }

    Rectangle baseRect = Rectangle { rect.x, rect.y, rect.width, rect.height };
    baseRect.y += 3*24;
    baseRect.height = 24;

    FileSystemService& fs = Editor::getFileSystem();
    std::vector<std::string> mapPaths = fs.getProject()->getMapPaths();
    for (std::string mapPath : mapPaths) {
        std::string mapFileName = GetFileNameWithoutExt(mapPath.c_str());
        if (GuiLabelButton(baseRect, mapFileName.c_str())) {
            if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
                fs.openProjectFile(mapPath);
                ui.setInitial();
            }
        }
        baseRect.y += 24;
    }
}

void ResourceViewerBox::drawActors()
{
    EditorInterfaceService& ui = Editor::getUi();
    WindowContainer& windows = ui.getWindowContainer();

    if (GuiButton(Rectangle { rect.x + 8, rect.y + 2*24, rect.width - 16, 24 }, "New..")) {
        if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
            windows.openActorInit();
        }
    }

    Rectangle baseRect = Rectangle { rect.x, rect.y, rect.width, rect.height };
    baseRect.y += 3*24;
    baseRect.height = 24;

    FileSystemService& fs = Editor::getFileSystem();
    std::vector<std::string> actorPaths = fs.getProject()->getActorPaths();
    for (std::string actorPath : actorPaths) {
        std::string actorFileName = GetFileNameWithoutExt(actorPath.c_str());
        if (GuiLabelButton(baseRect, actorFileName.c_str())) {
            if (ui.getMouseBoxLayer() == VIEWBOX_LAYER_BASE) {
                fs.openProjectFile(actorPath);
                ui.setInitial();
            }
        }
        baseRect.y += 24;
    }
}