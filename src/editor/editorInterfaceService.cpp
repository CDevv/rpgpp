#include "editorInterfaceService.hpp"

#include <TGUI/AllWidgets.hpp>
#include <TGUI/Backend/raylib.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/MenuBar.hpp>
#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <raylib.h>

#include "TGUI/Cursor.hpp"
#include "TGUI/Exception.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/Widgets/MessageBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "gamedata.hpp"
#include "nfd.h"
#include "resourceViewerBox.hpp"
#include "windowContainer.hpp"
#include "windows/projectBinaryViewWindow.hpp"

EditorInterfaceService::EditorInterfaceService() {
  gui = std::make_unique<tgui::Gui>();

  // Load the theme file from the config.
  ConfigurationFileService &confService = Editor::getConfigService();
  auto map_ref = confService.ini_structure["window"]["theme_file"];

  try {
    // Try to set the theme.
    tgui::Theme::setDefault(map_ref);
  } catch (tgui::Exception &e) {
    printf("tgui::Exception: %s \n", e.what());
    /*
      TODO:
      Add messageBoxes that actually show the user what's wrong.
    */
  }

  // Menu
  auto menu = tgui::MenuBar::create();
  menu->addMenu("File");
  menu->addMenuItem("New Project"); // TODO: to be implemted
  menu->addMenuItem("Open Project");
  menu->addMenu("Project");
  menu->addMenuItem("Open Project");
  menu->addMenuItem("Open Binary");
  menu->addMenu("Themes");
  menu->addMenuItem("Select from file");
  menu->addMenu("Developer");
  menu->addMenuItem("Open TGUI Website");
  menu->addMenu("Help");
  menu->addMenuItem("About RPG++");
  menu->connectMenuItem({"Themes", "Select from file"}, [this] {
    getWindowContainer().open("OpenThemeFileWindow");
  });
  menu->connectMenuItem({"File", "Open Project"}, [] {
    FileSystemService &fs = Editor::getFileSystem();
    fs.promptOpenProject();
  });
  menu->connectMenuItem({"Project", "Open Project"}, [] {
    FileSystemService &fs = Editor::getFileSystem();
    fs.promptOpenProject();
  });
  menu->connectMenuItem({"Project", "Open Binary"}, [this] {
    FileSystemService &fs = Editor::getFileSystem();
    ProjectBinaryViewWindow &projectBinary =
        getWindowContainer().openProjectBinaryView();
    FS_Result fsResult = fs.openGameData();
    if (fsResult.result == NFD_OKAY) {
      GameData gameData = deserializeFile(fsResult.path);
      projectBinary.setData(gameData);
    }
  });
  menu->connectMenuItem({"Developer", "Open TGUI Website"},
                        [] { OpenURL("https://tgui.eu"); });
  menu->connectMenuItem({"Help", "About RPG++"},
                        [this] { getWindowContainer().open("About"); });

  auto btn = tgui::Button::create("Title.");
  btn->setPosition({40, 40});
  gui->add(btn, "button");
  gui->add(menu);

  demoGuiActive = false;

  // get codepoints
  std::vector<int> codepoints;
  // ASCII latin
  for (int i = 0; i < 92; i++) {
    codepoints.push_back(i + 32);
  }
  // cyrillic
  for (int i = 0x400; i <= 0x4FF; i++) {
    codepoints.push_back(i);
  }

  // load the font
  uiFont = LoadFontEx("resources/LanaPixel.ttf", 13, codepoints.data(),
                      static_cast<int>(codepoints.size()));

  mousePos = Vector2{0, 0};
  hoverPos = Vector2{0, 0};
  mouseLock = false;
  mouseBoxLayer = VIEWBOX_LAYER_BASE;

  auto projectMenuRect =
      Rectangle{0, 16, static_cast<float>(GetScreenWidth()), 36};
  projectMenu = ProjectMenuBox(projectMenuRect);

  float elementBaseY = projectMenuRect.y + projectMenuRect.height;

  auto propRect = Rectangle{4, elementBaseY + 4, 160,
                            GetScreenHeight() - (elementBaseY + 8)};
  resourceView = ResourceViewerBox(propRect);

  auto tabListRect = Rectangle{propRect.x + propRect.width + 4, propRect.y,
                               GetScreenWidth() - propRect.width - 12, 24};
  tabList = TabList(tabListRect);

  auto windowRect = Rectangle{
      propRect.width + 8, elementBaseY + 4 + tabListRect.height + 4,
      GetScreenWidth() - (propRect.width + 16) + 4,
      GetScreenHeight() - (elementBaseY + 8) - (tabListRect.height + 4)};
  panelView = std::make_unique<PanelView>(windowRect);
  windowContainer = std::make_unique<WindowContainer>();
}

EditorInterfaceService::~EditorInterfaceService() = default;

tgui::Gui *EditorInterfaceService::getGui() { return gui.get(); }

void EditorInterfaceService::setInitial() { panelView->setInitial(); }

void EditorInterfaceService::unload() {
  windowContainer.reset(nullptr);
  gui.reset(nullptr);

  UnloadFont(uiFont);
  FileSystemService &fs = Editor::getFileSystem();
  if (fs.projectIsOpen()) {
    std::filesystem::path fPath =
        std::string(fs.getProject()->getProjectBasePath())
            .append("/")
            .append("run.lua");
    std::filesystem::remove(fPath);

#ifdef __linux__
    fs.getProject()->cleanCompilation();
#endif
  }
}

void EditorInterfaceService::update() {
  gui->handleEvents(); // Handles all non-keyboard events

  while (int pressedChar = GetCharPressed())
    gui->handleCharPressed(pressedChar);

  while (int pressedKey = GetKeyPressed())
    gui->handleKeyPressed(pressedKey);

  mainView.update();
  tabList.update();
  panelView->update();
  resourceView.update();

  windowContainer->update();
}

void EditorInterfaceService::draw() {
  gui->draw();
}

void EditorInterfaceService::drawMainView() {
  mainView.setRect(Rectangle{0, 0, static_cast<float>(GetScreenWidth()),
                             static_cast<float>(GetScreenHeight())});
  mainView.draw();
}

void EditorInterfaceService::drawProjectView() {
  FileSystemService &fs = Editor::getFileSystem();

  auto projectMenuRect =
      Rectangle{0, 16, static_cast<float>(GetScreenWidth()), 36};
  projectMenu.setRect(projectMenuRect);
  projectMenu.draw();

  if (fs.projectIsOpen()) {
    tabList.setRect(
        Rectangle{static_cast<float>(GetScreenWidth() * 0.2) + 8, 36 + 16 + 4,
                  static_cast<float>(GetScreenWidth() * 0.8) - (8 * 2), 18});
    tabList.draw();
  }

  panelView->setRect(Rectangle{
      static_cast<float>(GetScreenWidth() * 0.2) + 4, (36 + 24 + 18 + 4),
      static_cast<float>(GetScreenWidth() * 0.8) - 8,
      static_cast<float>(GetScreenHeight() - (36 + 24 + 18 + 4 + 4))});
  panelView->draw();

  resourceView.setRect(
      Rectangle{0, 36 + 18, static_cast<float>(GetScreenWidth() * 0.2),
                static_cast<float>(GetScreenHeight() - (38 + 18 + 4))});
  resourceView.draw();
}

Font EditorInterfaceService::getFont() const { return uiFont; }

WindowContainer &EditorInterfaceService::getWindowContainer() {
  return *windowContainer;
}

TabList &EditorInterfaceService::getTabList() { return tabList; }

void EditorInterfaceService::setMouseLock(bool value) {
  this->mouseLock = value;
}

bool EditorInterfaceService::getMouseLock() const { return this->mouseLock; }

void EditorInterfaceService::setMouseBoxLayer(ViewBoxLayer boxLayer) {
  this->mouseBoxLayer = boxLayer;
}

ViewBoxLayer EditorInterfaceService::getMouseBoxLayer() const {
  return this->mouseBoxLayer;
}
