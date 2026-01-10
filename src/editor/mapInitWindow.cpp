#include "windows/mapInitWindow.hpp"
#include <cstring>
#include <memory>
#include <raylib.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "room.hpp"
#include "tilemap.hpp"
#include "windowPopup.hpp"

MapInitWindow::MapInitWindow() : rect(), title{}, titleEditMode(false), hasSetTileSet(false)
{
}

MapInitWindow::MapInitWindow(Rectangle rect)
: WindowPopup("New Room..", rect)
{
    this->rect = rect;

    this->titleText = "";
    strcpy(title, "");
    this->titleEditMode = false;
    this->tileSetSource = "";
    this->hasSetTileSet = false;
}

void MapInitWindow::openWindow()
{
    WindowPopup::openWindow();
}

void MapInitWindow::closeWindow()
{
    EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
    ui.setMouseLock(false);

    this->titleText = "";
    strcpy(title, "");
    this->titleEditMode = false;
    this->tileSetSource = "";
    this->hasSetTileSet = false;

    WindowPopup::closeWindow();
}

void MapInitWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("New Room..")) {
		ImGui::InputText("Title", title, 256);

		tileSetSource.push_back('\0');
		ImGui::InputText("TileSet", tileSetSource.data(), tileSetSource.size());

		if (ImGui::Button("Change TileSet", ImVec2(-1, 0))) {
            FS_Result fsResult = fs.openTileSetResource();
            if (fsResult.result == NFD_OKAY) {
                tileSetSource = fsResult.path;
                hasSetTileSet = true;
            }
		}

		if (ImGui::Button("Submit")) {
			titleText = title;

			bool validated = !titleText.empty() && hasSetTileSet;

			if (validated) {
    			std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>(tileSetSource, 25, 25, 16, 48);

                Room room(std::move(tileMap));
                nlohmann::json roomJson = room.dumpJson();
                std::string jsonString = roomJson.dump(4);

                std::string filePath = std::string("maps/").append(titleText).append(".rmap");
                SaveFileText(filePath.c_str(), const_cast<char*>(jsonString.data()));

                fs.getProject()->reloadPaths();
                fs.openProjectFile(filePath);
                ui.setInitial();

    			closeWindow();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			closeWindow();
		}

	    ImGui::EndPopup();
	}
}
