#include "projectBinaryViewWindow.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include <raygui.h>

ProjectBinaryViewWindow::ProjectBinaryViewWindow() {}

ProjectBinaryViewWindow::ProjectBinaryViewWindow(Rectangle rect)
{
	this->rect = rect;
	this->active = false;
	this->dataAvailable = false;
	this->data = std::unique_ptr<GameData>{};
	this->fileType = FILE_TILESET;
	this->currentPage = FILE_TILESET;
	this->currentPageNum = 0;
	this->pageEditMode = false;

	Rectangle viewRect = Rectangle
	{
		rect.x + 140, rect.y + 28,
		rect.width - 144, rect.height - 32
	};
	this->tilesView = std::make_unique<WorldViewBox>(viewRect, FILE_TILESET, VIEWBOX_LAYER_BINVIEW);
	this->roomView = std::make_unique<WorldViewBox>(viewRect, FILE_ROOM, VIEWBOX_LAYER_BINVIEW);
}

void ProjectBinaryViewWindow::setActive()
{
	this->active = true;
}

void ProjectBinaryViewWindow::closeWindow()
{
	EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseLock(false);
    ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);

	this->dataAvailable = false;
	this->fileType = FILE_TILESET;
	this->currentPage = FILE_TILESET;
	this->currentPageNum = 0;
	this->data.reset();
	this->tileset.reset();
	this->room.reset();
	this->active = false;
}

void ProjectBinaryViewWindow::draw()
{
	this->tilesView->update();
	this->roomView->update();

	if (pageEditMode) GuiLock();

	if (active) {
		if (dataAvailable) {
			if (GuiWindowBox(rect, data->title.c_str())) {
				closeWindow();
			}

			if (dataAvailable) {
				switch (fileType) {
				case FILE_TILESET:
					this->tilesView->draw();
					break;
				case FILE_ROOM:
					this->roomView->draw();
					break;
				default:
					break;
				}

				drawResourcesList();
			}
		} else {
			if (GuiWindowBox(rect, "GameData")) {
				closeWindow();
			}
		}

		if (active) {
			EditorInterfaceService& ui = Editor::getUi();

			if (CheckCollisionPointRec(GetMousePosition(), rect)) {
				ui.setMouseBoxLayer(VIEWBOX_LAYER_BINVIEW);
                ui.setMouseLock(true);
            } else {
            	ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
                ui.setMouseLock(false);
            }
		}
	}
}

void ProjectBinaryViewWindow::setTileSet(std::string name)
{
	auto tileSetData = data->tilesets.at(name);

	unsigned char* imageData = tileSetData.image.data();
	Image image = LoadImageFromMemory(tileSetData.extension.c_str(), imageData, tileSetData.dataSize);
	Texture texture = LoadTextureFromImage(image);

	Vector2 tileSizeVec = Vector2 { static_cast<float>(tileSetData.tileSize.x), static_cast<float>(tileSetData.tileSize.y) };
	this->tileset = std::make_unique<TileSet>(texture, tileSizeVec);

	this->tilesView->setTileSet(tileset.get());
	this->fileType = FILE_TILESET;
}

void ProjectBinaryViewWindow::setRoom(RoomBin roomBin)
{
	//first load the tileset
	//setTileSet(roomBin.tileSetName);
	auto tileSetData = data->tilesets.at(roomBin.tileSetName);
	unsigned char* imageData = tileSetData.image.data();
	Image image = LoadImageFromMemory(tileSetData.extension.c_str(), imageData, tileSetData.dataSize);
	Texture texture = LoadTextureFromImage(image);
	Vector2 tileSizeVec = Vector2 { static_cast<float>(tileSetData.tileSize.x), static_cast<float>(tileSetData.tileSize.y) };
	std::unique_ptr<TileSet> tileSet = std::make_unique<TileSet>(texture, tileSizeVec);

	//load tilemap
	std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>(std::move(tileSet), roomBin.width, roomBin.height, 16, 48);
	for (std::vector<TileBin> row : roomBin.tiles) {
		for (TileBin col : row) {
			Vector2 atlasPos = Vector2 { static_cast<float>(col.atlasPos.x / tileSetData.tileSize.x), static_cast<float>(col.atlasPos.y / tileSetData.tileSize.y) };
			Vector2 worldPos = Vector2 { static_cast<float>(col.worldPos.x), static_cast<float>(col.worldPos.y) };
			tileMap->setTile(worldPos, atlasPos);
		}
	}

	//load room
	this->room = std::make_unique<Room>(std::move(tileMap));

	this->roomView->setRoom(room.get());
	this->fileType = FILE_ROOM;
}

void ProjectBinaryViewWindow::setData(GameData data)
{
	this->data = std::make_unique<GameData>(data);
	this->dataAvailable = true;

	//load first tileset
	setTileSet("tiles.rtiles");
}

void ProjectBinaryViewWindow::drawResourcesList()
{
	GuiPanel(Rectangle { rect.x + 4, rect.y + 28, 132, rect.height - 32 }, "Resources");

	Rectangle labelBaseRect = Rectangle
	{
		rect.x + 8, rect.y + 52 + 24,
		112, 24
	};
	switch (currentPage) {
	case FILE_TILESET:
		for (auto [name, tileSetData] : data->tilesets) {
			if (GuiLabelButton(labelBaseRect, tileSetData.name.c_str())) {
				setTileSet(name);
			}
			labelBaseRect.y += 24;
		}
		break;
	case FILE_ROOM:
		for (auto roomData : data->rooms) {
			if (GuiLabelButton(labelBaseRect, roomData.name.c_str())) {
				setRoom(roomData);
			}
			labelBaseRect.y += 24;
		}
		break;
	default:
		break;
	}

	Rectangle dropdownRect = Rectangle
	{
		rect.x + 8, rect.y + 52,
		122, 24
	};
	if (GuiDropdownBox(dropdownRect, "TileSets;Rooms", &currentPageNum, pageEditMode)) {
		pageEditMode = !pageEditMode;
	}
	currentPage = static_cast<EngineFileType>(currentPageNum);
}