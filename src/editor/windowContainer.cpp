#include "windowContainer.hpp"

#include <memory>
#include <raylib.h>

#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "propInitWindow.hpp"
#include "windowPopup.hpp"
#include "windows/aboutWindow.hpp"
#include "windows/actorInitWindow.hpp"
#include "windows/deleteConfirmWindow.hpp"
#include "windows/dialogueInitWindow.hpp"
#include "windows/errorWindow.hpp"
#include "windows/mapInitWindow.hpp"
#include "windows/openThemeFileWindow.hpp"
#include "windows/projectBinaryViewWindow.hpp"
#include "windows/projectInitWindow.hpp"
#include "windows/tileSetDialogWindow.hpp"
#include "windows/tileSetInitWindow.hpp"
#include "worldViewBox.hpp"

WindowContainer::WindowContainer() {
  windowOpen = false;
  auto baseTileSetSize = Rectangle{0, 0, 312, 184};
  auto tileSetWindowSize =
      Rectangle{(GetScreenWidth() - baseTileSetSize.width) / 2,
                (GetScreenHeight() - baseTileSetSize.height) / 2,
                baseTileSetSize.width, baseTileSetSize.height};

  auto baseProjectViewWindowSize =
      Rectangle{0, 0, static_cast<float>(GetScreenWidth() - 64),
                static_cast<float>(GetScreenHeight() - 64)};
  auto projectViewWindowRect = Rectangle{
      (GetScreenWidth() - baseProjectViewWindowSize.width) / 2,
      (GetScreenHeight() - baseProjectViewWindowSize.height) / 2,
      baseProjectViewWindowSize.width, baseProjectViewWindowSize.height};
  projectBinaryView = ProjectBinaryViewWindow(projectViewWindowRect);

  auto baseTileSetDialogSize = Rectangle{0, 0, 434, 404};
  auto tileSetDialogSize =
      Rectangle{(GetScreenWidth() - baseTileSetDialogSize.width) / 2,
                (GetScreenHeight() - baseTileSetDialogSize.height) / 2,
                baseTileSetDialogSize.width, baseTileSetDialogSize.height};
  tileSetDialog = TileSetDialogWindow(tileSetDialogSize);

  mapW = std::map<std::string, std::unique_ptr<WindowPopup>>{};

  mapW.try_emplace("ProjectInit", new ProjectInitWindow(tileSetWindowSize));
  mapW.try_emplace("TileSetInit", new TileSetInitWindow(tileSetWindowSize));
  mapW.try_emplace("MapInit", new MapInitWindow(tileSetWindowSize));
  mapW.try_emplace("ActorInit", new ActorInitWindow(tileSetWindowSize));
  mapW.try_emplace("About", new AboutWindow(tileSetWindowSize));
  mapW.try_emplace("Error", new ErrorWindow(Rectangle{20, 20, 240, 30}));
  mapW.try_emplace("DeleteConfirm", new DeleteConfirmWindow(tileSetWindowSize));
  mapW.try_emplace("DialogueInit", new DialogueInitWindow(tileSetWindowSize));
  mapW.try_emplace("PropInit", new PropInitWindow(tileSetWindowSize));
  mapW.try_emplace("OpenThemeFileWindow",
                   new OpenThemeFileWindow(tileSetDialogSize));
}

void WindowContainer::update() { tileSetDialog.update(); }

bool WindowContainer::isWindowOpen() const { return this->windowOpen; }

void WindowContainer::setWindowOpen(bool value) { this->windowOpen = value; }

ProjectBinaryViewWindow &WindowContainer::openProjectBinaryView() {
  windowOpen = true;
  projectBinaryView.setActive();
  return projectBinaryView;
}

TileSetDialogWindow &WindowContainer::openTileSetDialog() {
  EditorInterfaceService &ui = Editor::getUi();
  ui.setMouseBoxLayer(VIEWBOX_LAYER_DIALOG);
  windowOpen = true;
  tileSetDialog.setActive();
  return tileSetDialog;
}

void WindowContainer::draw() {
  drawWindow("TileSetInit");
  drawWindow("MapInit");
  drawWindow("ActorInit");
  drawWindow("DialogueInit");
  drawWindow("PropInit");
  drawWindow("OpenThemeFileWindow");

  if (!windowOpen) {
    EditorInterfaceService &ui = Editor::getUi();
    ui.setMouseLock(false);
  }
}

void WindowContainer::drawTileSetDialog() { tileSetDialog.draw(); }

void WindowContainer::drawProjectBinaryView() { projectBinaryView.draw(); }

WindowPopup &WindowContainer::open(const std::string &id) {
  mapW[id]->openWindow();
  return *mapW[id].get();
}

void WindowContainer::drawWindow(const std::string &id) { mapW[id]->draw(); }
