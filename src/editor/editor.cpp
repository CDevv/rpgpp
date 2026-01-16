#include "editor.hpp"
#include "editorGuiService.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>

Editor *Editor::instance;
Editor::Editor() {
  Editor::instance = this;
  this->guiService = std::make_unique<EditorGuiService>();
  this->translationService = std::make_unique<TranslationService>();
}

void Editor::setAppIcon(const char *icon_path) const {
  // half of the fucking things are auto
  auto img_loader = LoadImage(icon_path);
  Editor::instance->appIcon = img_loader;
  SetWindowIcon(img_loader);
}

void Editor::unload() const {
  // Unload all the assets currently loaded and close the window.
  UnloadImage(this->appIcon);
  CloseWindow();
}