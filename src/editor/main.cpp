#include "editor.hpp"
#include "editorGuiService.hpp"

int main() {
	auto editor = std::make_unique<Editor>();
	auto &gui = editor->guiService;

	gui->init();
	gui->uiLoop();
	editor->unload();

	return 0;
}