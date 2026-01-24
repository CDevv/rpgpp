#include "fileViews/tilesetFileView.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ListView.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "editor.hpp"
#include "propertiesBox.hpp"
#include "tileSetView.hpp"
#include "tileset.hpp"
#include "variant.hpp"
#include "worldView.hpp"
#include <algorithm>

void TileSetFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<TileSet> *>(variant);
		const auto tileset = ptr->get();

		const auto worldView = TileSetView::create(tileset);
		worldView->setSize({"100% - 300", "100%"});
		layout->add(worldView);
		Editor::instance->getGui().addUpdate(WorldView::asUpdatable(worldView));

		auto props = PropertiesBox::create();
		props->setSize({"300", "100%"});
		props->setPosition({"100% - 300", 0});
		props->addButton("Square Tiles", [tileset] {
			Vector2 tileSize = tileset->getTileSize();
			float min = std::min(tileSize.x, tileSize.y);
			tileset->setTileSizeVector({min, min});
		});
		props->addIntField("Tile Width",
						   static_cast<int>(tileset->getTileSize().x),
						   [tileset](float v) {
							   Vector2 tileSize = tileset->getTileSize();
							   tileSize.x = v;
							   tileset->setTileSizeVector(tileSize);
						   });
		props->addIntField("Tile Height",
						   static_cast<int>(tileset->getTileSize().y),
						   [tileset](float v) {
							   Vector2 tileSize = tileset->getTileSize();
							   tileSize.y = v;
							   tileset->setTileSizeVector(tileSize);
						   });
		layout->add(props);

		layout->add(props);
	}
}
