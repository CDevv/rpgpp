#include "widgets/roomView.hpp"
#include "TGUI/Vector2.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tile.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "widgets/roomToolbox.hpp"
#include "worldView.hpp"
#include <cmath>
#include <cstdio>
#include <memory>

RoomView::Ptr RoomView::create() { return std::make_shared<RoomView>(); }

RoomView::Ptr RoomView::create(Room *room) {
	auto ptr = std::make_shared<RoomView>();
	ptr->room = room;
	return ptr;
}

void RoomView::setRoom(Room *room) { this->room = room; }

Room *RoomView::getRoom() { return room; }

IVector RoomView::getTileAtMouse() {
	TileMap *tileMap = room->getTileMap();
	return {static_cast<int>(
				std::floor(mouseWorldPos.x / tileMap->getWorldTileSize())),
			static_cast<int>(
				std::floor(mouseWorldPos.y / tileMap->getWorldTileSize()))};
}

Rectangle RoomView::getSourceRect(TileMap *tileMap, int x, int y) {
	Rectangle sourceRect = {static_cast<float>(x * tileMap->getAtlasTileSize()),
							static_cast<float>(y * tileMap->getAtlasTileSize()),
							static_cast<float>(tileMap->getAtlasTileSize()),
							static_cast<float>(tileMap->getAtlasTileSize())};
	return sourceRect;
}

Rectangle RoomView::getDestRect(TileMap *tileMap, int x, int y) {
	Rectangle destRect = {static_cast<float>(x * tileMap->getWorldTileSize()),
						  static_cast<float>(y * tileMap->getWorldTileSize()),
						  static_cast<float>(tileMap->getWorldTileSize()),
						  static_cast<float>(tileMap->getWorldTileSize())};
	return destRect;
}

void RoomView::drawOverlay() {
	if (room == nullptr) {
		return;
	}

	TileMap *tileMap = room->getTileMap();
	Vector2 worldSize = tileMap->getMaxWorldSize();
	Vector2 actualSize = worldSize;
	actualSize.x *= tileMap->getWorldTileSize();
	actualSize.y *= tileMap->getWorldTileSize();

	Rectangle rect = {0, 0, actualSize.x, actualSize.y};

	if (CheckCollisionPointRec(mouseWorldPos, rect)) {
		int tileX = std::floor(mouseWorldPos.x / tileMap->getWorldTileSize());
		int tileY = std::floor(mouseWorldPos.y / tileMap->getWorldTileSize());
		DrawText(TextFormat("Tile [%i, %i]", tileX, tileY), 8, 8, 36, BLACK);
	}
}

void RoomView::drawCanvas() {
	ClearBackground(RAYWHITE);

	if (room == nullptr) {
		return;
	}

	TileMap *tileMap = room->getTileMap();
	if (tileMap == nullptr) {
		return;
	}
	TileSet *tileSet = tileMap->getTileSet();
	if (tileSet == nullptr) {
		return;
	}

	const Texture2D &texture = tileSet->getTexture();

	Vector2 worldSize = tileMap->getMaxWorldSize();
	Vector2 tileSize = {static_cast<float>(tileMap->getWorldTileSize()),
						static_cast<float>(tileMap->getWorldTileSize())};
	Vector2 atlasTileSize = {static_cast<float>(tileMap->getAtlasTileSize()),
							 static_cast<float>(tileMap->getAtlasTileSize())};
	int worldWidth = static_cast<int>(worldSize.x);
	int worldHeight = static_cast<int>(worldSize.y);
	for (int tileX = 0; tileX < worldWidth; tileX++) {
		for (int tileY = 0; tileY < worldHeight; tileY++) {
			auto tile = tileMap->getTile(tileX, tileY);
			auto atlasTile = tile.getAtlasTile();
			auto atlas = atlasTile.getAtlasCoords();

			Rectangle sourceRect = getSourceRect(tileMap, atlas.x, atlas.y);
			Rectangle destRect = getDestRect(tileMap, tileX, tileY);

			if (tile.isPlaced()) {
				// Draw tile itself
				DrawTexturePro(texture, sourceRect, destRect, {0.0f, 0.0f},
							   0.0f, WHITE);
			}

			if (tileSetView != nullptr) {
				handleMode(tileX, tileY);
			}

			// Draw tile border
			DrawRectangleLinesEx(destRect, 1.0f, Fade(GRAY, 0.5f));
			if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
				DrawRectangleLinesEx(destRect, 2.0f, Fade(GRAY, 0.5f));
			}
		}
	}

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}

void RoomView::mouseMoved(tgui::Vector2f pos) {
	if (brushMode && mouseLeftButton) {
		handleModePress(pos);
	}
	WorldView::mouseMoved(pos);
}

bool RoomView::leftMousePressed(tgui::Vector2f pos) {
	TileMap *tileMap = room->getTileMap();

	handleModePress(pos);

	return WorldView::leftMousePressed(pos);
}

void RoomView::handleMode(int x, int y) {
	switch (tool) {
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

void RoomView::handlePlaceMode(int x, int y) {
	TileMap *tileMap = room->getTileMap();
	TileSet *tileSet = tileMap->getTileSet();
	const Texture2D &texture = tileSet->getTexture();

	Rectangle destRect = getDestRect(tileMap, x, y);

	if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
		auto atlasTilePos = tileSetView->getSelectedTile();

		if (tileSet->areAtlasCoordsValid(
				{static_cast<float>(atlasTilePos.x),
				 static_cast<float>(atlasTilePos.y)})) {
			Rectangle atlasSourceRect =
				getSourceRect(tileMap, atlasTilePos.x, atlasTilePos.y);

			IVector tileMouse = getTileAtMouse();
			Rectangle destRect = getDestRect(tileMap, tileMouse.x, tileMouse.y);

			DrawTexturePro(texture, atlasSourceRect, destRect, {0.0f, 0.0f},
						   0.0f, Fade(WHITE, 0.7f));
		}
	}
}

void RoomView::handleEditMode(int x, int y) {
	TileMap *tileMap = room->getTileMap();
	Rectangle destRect = getDestRect(tileMap, x, y);

	if (selectedTile.x == x && selectedTile.y == y) {
		Rectangle selectedTileRect = getDestRect(tileMap, x, y);
		DrawRectangleRec(selectedTileRect, Fade(BLUE, 0.3f));
	}

	if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
		DrawRectangleLinesEx(destRect, 2.0f, ORANGE);
	}
}

void RoomView::handleEraseMode(int x, int y) {
	TileMap *tileMap = room->getTileMap();
	Rectangle destRect = getDestRect(tileMap, x, y);

	if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
		DrawRectangleRec(destRect, Fade(RED, 0.3f));
	}
}

void RoomView::handleModePress(tgui::Vector2f pos) {
	switch (tool) {
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

void RoomView::handlePlacePress(tgui::Vector2f pos) {
	TileMap *tileMap = room->getTileMap();

	IVector atlasTilePos = tileSetView->getSelectedTile();
	if (tileMap->getTileSet()->areAtlasCoordsValid(
			{static_cast<float>(atlasTilePos.x),
			 static_cast<float>(atlasTilePos.y)})) {
		IVector tileMouse = getTileAtMouse();

		Vector2 worldPos = {static_cast<float>(tileMouse.x),
							static_cast<float>(tileMouse.y)};
		Vector2 atlasPos = {static_cast<float>(atlasTilePos.x),
							static_cast<float>(atlasTilePos.y)};

		tileMap->setTile(worldPos, atlasPos);
	}
}

void RoomView::handleErasePress(tgui::Vector2f pos) {
	TileMap *tileMap = room->getTileMap();

	IVector tileMouse = getTileAtMouse();

	Vector2 worldPos = {static_cast<float>(tileMouse.x),
						static_cast<float>(tileMouse.y)};

	tileMap->setEmptyTile(worldPos);
}

void RoomView::handleEditPress(tgui::Vector2f pos) {
	TileMap *tileMap = room->getTileMap();

	selectedTile = getTileAtMouse();
	Vector2 atlasCoords = tileMap->getTile(selectedTile.x, selectedTile.y)
							  .getAtlasTile()
							  .getAtlasCoords();
	IVector atlasCoordsInt = {static_cast<int>(atlasCoords.x),
							  static_cast<int>(atlasCoords.y)};
	tileSetView->setSelectedTile(atlasCoordsInt);
	tileSetView->onTileSelected.disconnectAll();
	tileSetView->onTileSelected([this, tileMap](IVector newTile) {
		IVector tileMouse = selectedTile;

		if (tileMouse.x >= 0) {
			Vector2 worldPos = {static_cast<float>(tileMouse.x),
								static_cast<float>(tileMouse.y)};
			Vector2 atlasPos = {static_cast<float>(newTile.x),
								static_cast<float>(newTile.y)};

			tileMap->setTile(worldPos, atlasPos);
		}
	});
}

void RoomView::setBrush(bool value) { this->brushMode = value; }