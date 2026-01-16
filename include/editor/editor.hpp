#ifndef _RGPP_EDITOR_H
#define _RGPP_EDITOR_H
#include "editorGuiService.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>

class editor {

public:
  editor();
  // The RPG++ editor's current icon image.
  Image editor_icon;
  // the current editor instance.
  static editor *current_editor;
  // the current editor gui service, responsible for managing the gui.
  std::unique_ptr<editor_gui_service> gui_service;
  // the translation service responsible for all the i18n.
  std::unique_ptr<translation_service> ed_translation_service;
  // this sets the icon of the editor.
  void set_editor_icon(const char *icon_path) const;
  // unload editor stuff.
  void unload_editor() const;
};
#endif