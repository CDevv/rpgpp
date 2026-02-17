#include "fileViews/tilesetFileView.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "editor.hpp"
#include "widgets/propertiesBox.hpp"
#include "raylib.h"
#include "views/tileSetView.hpp"
#include "tileset.hpp"
#include "services/translationService.hpp"
#include "variant.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include "views/worldView.hpp"
#include <algorithm>

TileSetFileView::TileSetFileView() {
	TranslationService &ts = Editor::instance->getTranslations();

	const auto worldView = TileSetView::create();
	worldView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	widgetContainer.push_back(worldView);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(worldView));

	this->worldView = worldView;

	auto props = PropertiesBox::create();
	props->setSize({RIGHT_PANEL_W, "100%"});
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	widthField = IntField::create();
	widthField->label->setText(ts.getKey("screen.project.tilesetview.tile_width"));
	widthField->value->onValueChange([this](int value) {
		this->worldView->getTileSet()->setTileWidth(value);
	});
	props->addIntField(widthField);

	heightField = IntField::create();
	heightField->label->setText(ts.getKey("screen.project.tilesetview.tile_height"));
	heightField->value->onValueChange([this](int value) {
		this->worldView->getTileSet()->setTileHeight(value);
	});

	props->addButton("Square Tiles", [this] {
		auto tileset = this->worldView->getTileSet();
		Vector2 tileSize = tileset->getTileSize();
		float min = std::min(tileSize.x, tileSize.y);
		tileset->setTileSizeVector({min, min});

		widthField->value->setValue(min);
		heightField->value->setValue(min);
	});

	props->addIntField(widthField);
	props->addIntField(heightField);

	textureFile = FileField::create(ts.getKey("screen.project.tilesetview.texture"), "...");
	textureFile->pathFilters = {{
		{"Images", {"*.png", "*.jpg"}},
	}};
	textureFile->callback = [this](const tgui::String &path) {
		auto tileset = this->worldView->getTileSet();
		tileset->setTextureSource(path.toStdString());
	};
	props->addFileField(textureFile);

	widgetContainer.push_back(props);
}

void TileSetFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<TileSet> *>(variant);
		const auto tileset = ptr->get();

		worldView->setTileSet(tileset);

		widthField->value->setValue(tileset->getTileWidth());
		heightField->value->setValue(tileset->getTileHeight());
		textureFile->setValue(GetFileName(tileset->getTextureSource().c_str()));

		addWidgets(layout);
	}
}
