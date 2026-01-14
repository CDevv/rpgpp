#include "windows/mapInitWindow.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "room.hpp"
#include "tilemap.hpp"
#include "windowContainer.hpp"
#include "windowPopup.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <memory>
#include <raylib.h>

MapInitWindow::MapInitWindow()
    : rect(), title{}, titleEditMode(false), hasSetTileSet(false) {}

MapInitWindow::MapInitWindow(Rectangle rect) : WindowPopup("New Room..", rect) {
  this->rect = rect;

  this->titleText = "";
  strcpy(title, "");
  this->titleEditMode = false;
  this->tileSetSource = "";
  this->hasSetTileSet = false;
}

void MapInitWindow::openWindow() { WindowPopup::openWindow(); }

void MapInitWindow::closeWindow() {
  EditorInterfaceService &ui = Editor::getUi();
  ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
  ui.setMouseLock(false);

  this->titleText = "";
  strcpy(title, "");
  this->titleEditMode = false;
  this->tileSetSource = "";
  this->hasSetTileSet = false;

  WindowPopup::closeWindow();
}

void MapInitWindow::draw() {
  FileSystemService &fs = Editor::getFileSystem();
  EditorInterfaceService &ui = Editor::getUi();

  ImGui::SetNextWindowSize(ImVec2{rect.width, -1});
  if (ImGui::BeginPopupModal("New Room..")) {
    ImGui::InputText("Title", title, IM_ARRAYSIZE(title));

    tileSetSource.push_back('\0');
    ImGui::InputText("TileSet", tileSetSource.data(), tileSetSource.size());

    if (ImGui::Button("Change TileSet", ImVec2(-1, 0))) {
      FS_Result fsResult = fs.openTileSetResource();
      if (fsResult.result == NFD_OKAY) {
        tileSetSource = fsResult.path;
        hasSetTileSet = true;
      }
    }
    bool doesPlayerActorExist = std::filesystem::exists(DEFAULT_PLAYER_PATH);

    // If there is no player actor currently available, tell the user to make
    // one.

    // Check if the player actor does exist.
    if (doesPlayerActorExist) {
      if (ImGui::Button("Submit")) {
        titleText = title;

        bool validated = !titleText.empty() && hasSetTileSet;

        // we don't need it on the same line. shut up sonarqube.
        if (validated) {
          auto tileMap =
              std::make_unique<TileMap>(tileSetSource, 25, 25, 16, 48);

          Room room(std::move(tileMap));
          nlohmann::json roomJson = room.dumpJson();
          std::string jsonString = roomJson.dump(4);

          std::string filePath =
              std::string("maps/").append(titleText).append(".rmap");
          SaveFileText(filePath.c_str(), jsonString.data());

          fs.getProject()->reloadPaths();
          fs.openProjectFile(filePath);
          ui.setInitial();

          closeWindow();
        } else {
          printf("Not validated.. (mapInitWindow)\n");
        }
      }
      ImGui::SameLine();
    } else {
      ImGui::Text("You need a playerActor to create a map!");
    }

    if (ImGui::Button("Cancel")) {
      closeWindow();
    }

    ImGui::EndPopup();
  }
}
