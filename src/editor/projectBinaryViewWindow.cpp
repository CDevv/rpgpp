#include "projectBinaryViewWindow.hpp"
#include <raygui.h>

ProjectBinaryViewWindow::ProjectBinaryViewWindow() {}

ProjectBinaryViewWindow::ProjectBinaryViewWindow(Rectangle rect)
{
	this->rect = rect;
	this->active = false;
	this->dataAvailable = false;
	this->data = std::unique_ptr<GameData>{};

	Rectangle viewRect = Rectangle
	{
		rect.x + 140, rect.y + 28,
		rect.width - 144, rect.height - 32
	};
	this->tilesView = std::make_unique<WorldViewBox>(viewRect, FILE_TILESET);
}

void ProjectBinaryViewWindow::setActive()
{
	this->active = true;
}

void ProjectBinaryViewWindow::closeWindow()
{
	this->dataAvailable = false;
	this->data.reset();
	this->tileset.reset();
	this->active = false;
}

void ProjectBinaryViewWindow::draw()
{
	if (active) {
		if (dataAvailable) {
			if (GuiWindowBox(rect, data->title.c_str())) {
				closeWindow();
			}

			if (dataAvailable) {
				//DrawTexture(tileset->getTexture(), rect.x + 140, rect.y + 36, WHITE);
				this->tilesView->draw();

				GuiPanel(Rectangle { rect.x + 4, rect.y + 28, 132, rect.height - 32 }, "Resources");
				Rectangle labelBaseRect = Rectangle
				{
					rect.x + 8, rect.y + 52,
					112, 24
				};
				for (auto tileSetData : data->tilesets) {
					if (GuiLabelButton(labelBaseRect, tileSetData.name.c_str())) {
						unsigned char* imageData = tileSetData.image.data();
						Image image = LoadImageFromMemory(".png", imageData, tileSetData.dataSize);
						Texture texture = LoadTextureFromImage(image);

						tileset = std::make_unique<TileSet>(texture, tileSetData.tileSize);
						this->tilesView->setTileSet(tileset.get());
					}
					labelBaseRect.y += 24;
				}
			}
		} else {
			if (GuiWindowBox(rect, "GameData")) {
				closeWindow();
			}
		}
	}
}

void ProjectBinaryViewWindow::setData(GameData data)
{
	this->data = std::make_unique<GameData>(data);
	this->dataAvailable = true;

	//load first tileset
	auto tileSetData = data.tilesets.at(0);

	unsigned char* imageData = tileSetData.image.data();
	Image image = LoadImageFromMemory(".png", imageData, tileSetData.dataSize);
	Texture texture = LoadTextureFromImage(image);

	tileset = std::make_unique<TileSet>(texture, tileSetData.tileSize);

	this->tilesView->setTileSet(tileset.get());
}