#include "fileViews/tilesetFileView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "updatable.hpp"
#include "worldView.hpp"
#include <memory>

void TileSetFileView::init(tgui::Group::Ptr layout) {
	layout->add(tgui::Label::create("meow."));

	const auto worldView = WorldView::create();
	worldView->setSize({300, 300});
	layout->add(worldView);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(worldView));
}