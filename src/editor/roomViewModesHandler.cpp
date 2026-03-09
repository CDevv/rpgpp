#include "roomViewModesHandler.hpp"
#include "raylib.h"

RoomViewModesHandler::RoomViewModesHandler() {}

void RoomViewModesHandler::handleMode(int x, int y) {
	if (view.expired())
		return;

	auto ptr = view.lock();

	switch (ptr->tool) {
	case RoomTool::TOOL_PLACE:
		handlePlaceMode(x, y);
		break;
	case RoomTool::TOOL_ERASE:
		handleEraseMode(x, y);
		break;
	case RoomTool::TOOL_EDIT:
		handleEditMode(x, y);
		break;
	default:
		break;
	}
}

void RoomViewModesHandler::handlePlaceMode(int x, int y) {
	auto ptr = view.lock();

	TileMap *tileMap = ptr->room->getTileMap();
	TileSet *tileSet = tileMap->getTileSet();
	const Texture2D &texture = tileSet->getTexture();

	// Rectangle destRect = getDestRect(tileMap, x, y);
	Rectangle destRect = state.destRect;

	if (CheckCollisionPointRec(state.mouseWorldPos, destRect)) {
		auto atlasTilePos = ptr->tileSetView->getSelectedTile();

		if (tileSet->areAtlasCoordsValid(
				{static_cast<float>(atlasTilePos.x),
				 static_cast<float>(atlasTilePos.y)})) {
			Rectangle atlasSourceRect =
				ptr->getSourceRect(tileMap, atlasTilePos.x, atlasTilePos.y);

			IVector tileMouse = ptr->getTileAtMouse();
			Rectangle destRect =
				ptr->getDestRect(tileMap, tileMouse.x, tileMouse.y);

			DrawTexturePro(texture, atlasSourceRect, destRect, {0.0f, 0.0f},
						   0.0f, Fade(WHITE, 0.7f));
		}
	}
}

void RoomViewModesHandler::handleEditMode(int x, int y) {
	auto ptr = view.lock();

	TileMap *tileMap = ptr->room->getTileMap();
	Rectangle destRect = ptr->getDestRect(tileMap, x, y);

	if (ptr->selectedTile.x == x && ptr->selectedTile.y == y) {
		Rectangle selectedTileRect = ptr->getDestRect(tileMap, x, y);
		DrawRectangleRec(selectedTileRect, Fade(BLUE, 0.3f));
	}

	if (CheckCollisionPointRec(ptr->mouseWorldPos, destRect)) {
		DrawRectangleLinesEx(destRect, 2.0f, ORANGE);
	}
}

void RoomViewModesHandler::handleEraseMode(int x, int y) {
	auto ptr = view.lock();

	TileMap *tileMap = ptr->room->getTileMap();
	Rectangle destRect = ptr->getDestRect(tileMap, x, y);

	if (CheckCollisionPointRec(ptr->mouseWorldPos, destRect)) {
		DrawRectangleRec(destRect, Fade(RED, 0.3f));
	}
}

void RoomViewModesHandler::handleModePress(tgui::Vector2f pos) {
	if (view.expired())
		return;

	auto ptr = view.lock();

	switch (ptr->tool) {
	case RoomTool::TOOL_PLACE:
		handlePlacePress(pos);
		break;
	case RoomTool::TOOL_ERASE:
		handleErasePress(pos);
		break;
	case RoomTool::TOOL_EDIT:
		handleEditPress(pos);
		break;
	default:
		break;
	}
}

void RoomViewModesHandler::handlePlacePress(tgui::Vector2f pos) {
	auto ptr = view.lock();

	TileMap *tileMap = ptr->room->getTileMap();

	IVector atlasTilePos = ptr->tileSetView->getSelectedTile();
	if (tileMap->getTileSet()->areAtlasCoordsValid(
			{static_cast<float>(atlasTilePos.x),
			 static_cast<float>(atlasTilePos.y)})) {
		IVector tileMouse = ptr->getTileAtMouse();

		Vector2 worldPos = {static_cast<float>(tileMouse.x),
							static_cast<float>(tileMouse.y)};
		Vector2 atlasPos = {static_cast<float>(atlasTilePos.x),
							static_cast<float>(atlasTilePos.y)};

		tileMap->setTile(worldPos, atlasPos);
	}
}

void RoomViewModesHandler::handleErasePress(tgui::Vector2f pos) {
	auto ptr = view.lock();

	TileMap *tileMap = ptr->room->getTileMap();

	IVector tileMouse = ptr->getTileAtMouse();

	Vector2 worldPos = {static_cast<float>(tileMouse.x),
						static_cast<float>(tileMouse.y)};

	tileMap->setEmptyTile(worldPos);
}

void RoomViewModesHandler::handleEditPress(tgui::Vector2f pos) {
	auto ptr = view.lock();

	TileMap *tileMap = ptr->room->getTileMap();

	ptr->selectedTile = ptr->getTileAtMouse();
	Vector2 atlasCoords =
		tileMap->getTile(ptr->selectedTile.x, ptr->selectedTile.y)
			.getAtlasTile()
			.getAtlasCoords();
	IVector atlasCoordsInt = {static_cast<int>(atlasCoords.x),
							  static_cast<int>(atlasCoords.y)};
	ptr->tileSetView->setSelectedTile(atlasCoordsInt);
	ptr->tileSetView->onTileSelected.disconnectAll();
	ptr->tileSetView->onTileSelected([ptr, tileMap](IVector newTile) {
		IVector tileMouse = ptr->selectedTile;

		if (tileMouse.x >= 0) {
			Vector2 worldPos = {static_cast<float>(tileMouse.x),
								static_cast<float>(tileMouse.y)};
			Vector2 atlasPos = {static_cast<float>(newTile.x),
								static_cast<float>(newTile.y)};

			tileMap->setTile(worldPos, atlasPos);
		}
	});
}