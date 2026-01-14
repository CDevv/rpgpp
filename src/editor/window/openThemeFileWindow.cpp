#pragma once
#include "windows/openThemeFileWindow.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "editor/configurationFileService.hpp"
#include "fileSystemService.hpp"
#include "nfd.h"
#include "raylib.h"
#include "windowPopup.hpp"

OpenThemeFileWindow::OpenThemeFileWindow() : rect(Rectangle{}) {}

OpenThemeFileWindow::OpenThemeFileWindow(Rectangle rect)
    : WindowPopup("Open Theme from a file...", rect) {
  this->rect = rect;
  auto childWindow = tgui::ChildWindow::create("Theme Selector");
  auto buttonLayout = tgui::GrowVerticalLayout::create();
  // Create the window itself.
  childWindow->setSize({350, 150});
  buttonLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);
  buttonLayout->setPosition({"0%", "5%"});
  // Get the configuration file service, then show the current selected theme
  // file.
  ConfigurationFileService &configService = Editor::getConfigService();
  auto currentSelectedTheme = tgui::Label::create(
      configService.ini_structure[window_path_ref]["theme_file"]);
  currentSelectedTheme->setHorizontalAlignment(
      tgui::HorizontalAlignment::Center);
  // The theme selector button, responsible for selecting the theme.
  auto selectThemeButton = tgui::Button::create("Select theme from file...");
  selectThemeButton->onPress.connect([] {
    FileSystemService &fs = Editor::getFileSystem();
    FS_Result fs_result = fs.openThemeFile();
    if (fs_result.result == NFD_OKAY) {
      ConfigurationFileService &configFile = Editor::getConfigService();
      configFile.ini_structure["window"]["theme_file"] = fs_result.absolutePath;
      configFile.ini_file->write(configFile.ini_structure, true);
    }
  });
  selectThemeButton->setMouseCursor(tgui::Cursor::Type::Hand);
  // Change text.
  buttonLayout->add(currentSelectedTheme);
  buttonLayout->add(selectThemeButton);
  auto centeredWarningText =
      tgui::Label::create("You must restart RPG++ for the changes.");
  centeredWarningText->setHorizontalAlignment(
      tgui::HorizontalAlignment::Center);
  buttonLayout->add(centeredWarningText);
  childWindow->add(buttonLayout);
  this->childWindow = childWindow;
}

void OpenThemeFileWindow::openWindow() { WindowPopup::openWindow(); }

void OpenThemeFileWindow::closeWindow() { WindowPopup::closeWindow(); }

void OpenThemeFileWindow::draw() {}