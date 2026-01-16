#include "editor.hpp"
#include "editorGuiService.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>

editor *editor::current_editor;
editor::editor() {
  editor::current_editor = this;
  this->editor_gui_service = std::make_unique<class editor_gui_service>();
  this->translation_service = std::make_unique<class translation_service>();
}

void editor::set_editor_icon(const char *icon_path) const {
  // half of the fucking things are auto
  auto img_loader = LoadImage(icon_path);
  editor::current_editor->editor_icon = img_loader;
  SetWindowIcon(img_loader);
}

void editor::unload_editor() const {
  // Unload all the assets currently loaded and close the window.
  UnloadImage(this->editor_icon);
  CloseWindow();
}