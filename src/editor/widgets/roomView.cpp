#include "widgets/roomView.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tile.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
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

			// Draw tile border
			DrawRectangleLinesEx(destRect, 1.0f, Fade(GRAY, 0.5f));

			Vector2 toCheck = mouseWorldPos;
			// toCheck.y += getPosition().y;
			// toCheck.y += 32;
			if (CheckCollisionPointRec(toCheck, destRect)) {
				DrawRectangleLinesEx(destRect, 2.0f, Fade(GRAY, 0.5f));
			}
		}
	}

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}