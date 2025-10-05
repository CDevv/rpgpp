#include "tileSetInitWindow.hpp"
#include <cstring>
#include <raylib.h>
#include <imgui.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "nfd.h"
#include "tileset.hpp"

TileSetInitWindow::TileSetInitWindow() {}

TileSetInitWindow::TileSetInitWindow(Rectangle rect)
{
    this->active = false;
    this->rect = rect;

    strcpy(title, "");
    this->titleEditMode = false;
    this->titleText = "";
    this->hasSetTextureSource = false;
}

void TileSetInitWindow::setActive()
{
    active = true;
    ImGui::OpenPopup("New TileSet..");
}

void TileSetInitWindow::closeWindow()
{
    EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
    ui.setMouseLock(false);

    hasSetTextureSource = false;
    textureSource = "";
    titleText = "";
    strcpy(title, "");
    active = false;

    ImGui::CloseCurrentPopup();
}

void TileSetInitWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("New TileSet..")) {
		ImGui::InputText("Title", title, 256);

		textureSource.push_back('\0');
		ImGui::InputText("Texture", textureSource.data(), textureSource.size());

		if (ImGui::Button("Change Texture", ImVec2(-1, 0))) {
            FS_Result fsResult = fs.openImage();
            if (fsResult.result == NFD_OKAY) {
                textureSource = fsResult.path;
                hasSetTextureSource = true;
            }
		}

		if (ImGui::Button("Submit")) {
			titleText = title;

			bool validated = !titleText.empty() && hasSetTextureSource;

			if (validated) {
    			TileSet tileSet(textureSource, 16);
    			nlohmann::json tileSetJson = tileSet.dumpJson();
    			std::string jsonString = tileSetJson.dump(4);

    			std::string filePath = std::string("tilesets/").append(titleText).append(".rtiles");
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
