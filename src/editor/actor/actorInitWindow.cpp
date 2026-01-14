#include "windows/actorInitWindow.hpp"

#include <cstring>
#include <nlohmann/json.hpp>

#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "windowPopup.hpp"

ActorInitWindow::ActorInitWindow()
    : rect(), titleEditMode(false), title{}, hasSetTileSet(false) {}

ActorInitWindow::ActorInitWindow(Rectangle rect)
    : WindowPopup("New Actor..", rect) {
  this->rect = rect;

  this->titleEditMode = false;
  strcpy(this->title, "");
  this->titleText = "";
  this->hasSetTileSet = false;
  this->tileSetSource = "";
}

void ActorInitWindow::openWindow() { WindowPopup::openWindow(); }

void ActorInitWindow::closeWindow() {
  EditorInterfaceService &ui = Editor::getUi();
  ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
  ui.setMouseLock(false);

  strcpy(this->title, "");
  titleEditMode = false;
  titleText = "";
  hasSetTileSet = false;
  tileSetSource = "";

  WindowPopup::closeWindow();
}

void ActorInitWindow::draw() {
  FileSystemService &fs = Editor::getFileSystem();
  EditorInterfaceService &ui = Editor::getUi();

  ImGui::SetNextWindowSize(ImVec2{rect.width, -1});
  if (ImGui::BeginPopupModal("New Actor..")) {
    ImGui::InputText("Title", title, 256);
    ImGui::SameLine();
    if (ImGui::Button("Set as Player")) {
      strcpy(title, "playerActor");
    }
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
        std::unique_ptr<TileSet> tileSet =
            std::make_unique<TileSet>(tileSetSource);
        Actor actor(std::move(tileSet), Vector2{0, 0}, tileSetSource);

        nlohmann::json actorJson = actor.dumpJson();
        std::string jsonString = actorJson.dump(4);

        std::string filePath =
            std::string("actors/").append(titleText).append(".ractor");
        SaveFileText(filePath.c_str(), const_cast<char *>(jsonString.data()));

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
