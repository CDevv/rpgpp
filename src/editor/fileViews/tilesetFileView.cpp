#include "fileViews/tilesetFileView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "tileset.hpp"
#include "updatable.hpp"
#include "variant.hpp"
#include "worldView.hpp"
#include <memory>

void TileSetFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	auto label = tgui::Label::create("meow.");
	label->setPosition(0, 400);

	if (variant != nullptr) {
		auto ptr = dynamic_cast<Variant<TileSet> *>(variant);
		auto tileset = ptr->get();
		label->setText(tileset->getTextureSource());
	}

	layout->add(label);

	const auto worldView = WorldView::create();
	worldView->setSize({300, 300});
	layout->add(worldView);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(worldView));
}