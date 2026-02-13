#include "editor.hpp"
#include "services/editorGuiService.hpp"

int main() {
	const auto editor = std::make_unique<Editor>();
	auto &gui = editor->getGui();

	gui.init();
	gui.uiLoop();
	editor->unload();

	return 0;
}
