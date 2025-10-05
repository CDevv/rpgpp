#include "projectBinaryViewWindow.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include <imgui.h>
#include "tileset.hpp"
#include <memory>
#include <raylib.h>

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
		rect.x + 240 + 8, rect.y + 24,
		rect.width - (240 + 12), rect.height - 28
	};
	this->tilesView = std::make_unique<WorldViewBox>(viewRect, FILE_TILESET, VIEWBOX_LAYER_BINVIEW);
	this->roomView = std::make_unique<WorldViewBox>(viewRect, FILE_ROOM, VIEWBOX_LAYER_BINVIEW);
}

void ProjectBinaryViewWindow::setActive()
{
	this->active = true;

	EditorInterfaceService& ui = Editor::getUi();
	ui.setMouseBoxLayer(VIEWBOX_LAYER_BINVIEW);
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

	EditorInterfaceService& ui = Editor::getUi();

	float closeHeight = 20 - 8;
	Rectangle closeRect = Rectangle {
	    rect.x + (rect.width - closeHeight - 4), rect.y + 4,
		closeHeight, closeHeight
	};

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	if (active && dataAvailable) {
	    ImVec4 windowBg = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

		draw->AddRectFilled(ImVec2 { rect.x, rect.y },
		ImVec2 { rect.x + rect.width, rect.y + rect.height }, ImGui::ColorConvertFloat4ToU32(windowBg));
        draw->AddRectFilled(ImVec2 { rect.x, rect.y }, ImVec2 { rect.x + rect.width, rect.y + 20 }, IM_COL32_BLACK);
        draw->AddText(ImVec2 { rect.x + 4, rect.y + 4 }, IM_COL32_WHITE, TextFormat("Project Binary - %s", data->title.c_str()));
        drawCloseButton(draw);

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
	}
}

void ProjectBinaryViewWindow::drawResourcesList()
{
	ImGui::SetNextWindowPos(ImVec2 { rect.x + 4, rect.y + 24 });
	ImGui::SetNextWindowSize(ImVec2 { 240, rect.height - 20 - 8 });
	if (ImGui::Begin("Resources##BinResList", nullptr,
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
		ImGui::Combo("Page", &currentPageNum, "TileSets\0Rooms\0Actors\0");
		if (ImGui::BeginChild("ResourcesList", ImVec2(0, 0),
		    ImGuiChildFlags_Borders)) {
			switch (currentPage) {
			case FILE_TILESET:
				for (auto [name, tileSetData] : data->tilesets) {
					if (ImGui::Button(tileSetData.name.c_str())) {
						setTileSet(name);
					}
				}
				break;
			case FILE_ROOM:
				for (auto roomData : data->rooms) {
					if (ImGui::Button(roomData.name.c_str())) {
						setRoom(roomData);
					}
				}
				break;
			default:
				break;
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	currentPage = static_cast<EngineFileType>(currentPageNum);
}

void ProjectBinaryViewWindow::drawCloseButton(ImDrawList* draw)
{
    EditorInterfaceService& ui = Editor::getUi();
    ImVec4 closeColor = ImVec4 { 0, 0, 0, 0 };

    float closeHeight = 20 - 8;
	Rectangle closeRect = Rectangle {
	    rect.x + (rect.width - closeHeight - 4), rect.y + 4,
		closeHeight, closeHeight
	};

	if (CheckCollisionPointRec(GetMousePosition(), closeRect)) {
	    closeColor = ImVec4 { 0.4f, 0.4f, 0.4f, 1.0f };
	    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			closeWindow();
		}
	}

	draw->AddRectFilled(ImVec2 { closeRect.x, closeRect.y },
	ImVec2 { closeRect.x + closeRect.width, closeRect.y + closeRect.height }, ImGui::ColorConvertFloat4ToU32(closeColor));
	draw->AddImage((ImTextureRef)ui.getCloseTexture().id,
            ImVec2 { closeRect.x, closeRect.y }, ImVec2 { closeRect.x + closeRect.width, closeRect.y + closeRect.height });
}

void ProjectBinaryViewWindow::setTileSet(std::string name)
{
	auto tileSetData = data->tilesets.at(name);

	unsigned char* imageData = tileSetData.image.data();
	Image image = LoadImageFromMemory(tileSetData.extension.c_str(), imageData, tileSetData.dataSize);
	Texture texture = LoadTextureFromImage(image);

	Vector2 tileSizeVec = Vector2 { static_cast<float>(tileSetData.tileSize.x), static_cast<float>(tileSetData.tileSize.y) };
	//this->tileset = std::make_unique<TileSet>(texture, tileSizeVec);
	this->tileset = std::make_unique<TileSet>(tileSetData);

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
