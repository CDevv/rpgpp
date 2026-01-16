#include "editor.hpp"
#include "editorGuiService.hpp"

/*
    main editor entry point
*/

int main() {
  // Create the editor, and initialize it.
  auto c_editor = std::make_unique<editor>();
  // start the gui handler and other services.
  auto const &egs = c_editor->gui_service;
  egs->initialize_gui();
  egs->start_gui_loop();
  // unload the editor when it's finished.
  c_editor->unload_editor();
  return 0;
}