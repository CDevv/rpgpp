#ifndef _RGPP_EDITOR_H
#define _RGPP_EDITOR_H

#include "editorGuiService.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>

class Editor {

public:
  Editor();
  ~Editor() = default;
  // The RPG++ editor's current icon image.
  Image appIcon{};
  // the current editor instance.
  static Editor *instance;
  // the current editor gui service, responsible for managing the gui.
  std::unique_ptr<EditorGuiService> guiService;
  // the translation service responsible for all the i18n.
  std::unique_ptr<TranslationService> translationService;
  // this sets the icon of the editor.
  static void setAppIcon(const char *icon_path) ;
  // unload editor stuff.
  void unload() const;
};

#endif