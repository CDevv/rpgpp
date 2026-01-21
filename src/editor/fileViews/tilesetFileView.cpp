#include "fileViews/tilesetFileView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"

void TileSetFileView::init(tgui::Group::Ptr layout) {
	layout->add(tgui::Label::create("meow."));
}