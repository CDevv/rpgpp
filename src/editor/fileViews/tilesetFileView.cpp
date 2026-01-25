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
#include "fileField.hpp"
#include "intField.hpp"
#include "propertiesBox.hpp"
#include "raylib.h"
#include "tileSetView.hpp"
#include "tileset.hpp"
#include "variant.hpp"
#include "worldView.hpp"
#include <algorithm>
#include <cstdio>

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

		auto widthField = IntField::create();
		widthField->label->setText("Tile Height");
		widthField->value->setValue(tileset->getTileWidth());
		widthField->value->onValueChange(
			[tileset](int value) { tileset->setTileWidth(value); });
		props->addIntField(widthField);

		auto heightField = IntField::create();
		heightField->label->setText("Tile Height");
		heightField->value->setValue(tileset->getTileHeight());
		heightField->value->onValueChange(
			[tileset](int value) { tileset->setTileHeight(value); });

		props->addButton("Square Tiles", [tileset, widthField, heightField] {
			Vector2 tileSize = tileset->getTileSize();
			float min = std::min(tileSize.x, tileSize.y);
			tileset->setTileSizeVector({min, min});

			widthField->value->setValue(min);
			heightField->value->setValue(min);
		});

		props->addIntField(widthField);
		props->addIntField(heightField);

		auto textureFile = FileField::create(
			"Texture",
			std::string(GetFileName(tileset->getTextureSource().c_str())));
		textureFile->pathFilters = {{
			{"Images", {"*.png", "*.jpg"}},
		}};
		textureFile->callback = [tileset](const tgui::String &path) {
			tileset->setTextureSource(path.toStdString());
		};
		props->addFileField(textureFile);

		layout->add(props);

		layout->add(props);
	}
}
