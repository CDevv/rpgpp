#include "tileSetView.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "tileset.hpp"

TileSetView::Ptr TileSetView::create() {
	auto tileSetView = std::make_shared<TileSetView>();
	tileSetView->tileSet = nullptr;

	return tileSetView;
}

TileSetView::Ptr TileSetView::create(TileSet *tileSet) {
	auto tileSetView = std::make_shared<TileSetView>();
	tileSetView->tileSet = tileSet;

	return tileSetView;
}

void TileSetView::setTileSet(TileSet *newTileSet) {
	this->tileSet = newTileSet;
}

TileSet *TileSetView::getTileSet() { return tileSet; }

void TileSetView::drawOverlay() {
	if (this->tileSet == nullptr) {
		return;
	}

	const Texture2D &texture = this->tileSet->getTexture();

	Rectangle textureRect = {
		0, 0, static_cast<float>(texture.width * RPGPP_DRAW_MULTIPLIER),
		static_cast<float>(texture.height * RPGPP_DRAW_MULTIPLIER)};

	if (CheckCollisionPointRec(mouseWorldPos, textureRect)) {
		int atlasPosX = (mouseWorldPos.x /
						 (RPGPP_DRAW_MULTIPLIER * tileSet->getTileWidth()));
		int atlasPosY = (mouseWorldPos.y /
						 (RPGPP_DRAW_MULTIPLIER * tileSet->getTileHeight()));

		DrawText(TextFormat("Tile [%i, %i]", atlasPosX, atlasPosY), 8, 8, 32,
				 BLACK);
	}
}

void TileSetView::drawCanvas() {
	if (this->tileSet == nullptr) {
		return;
	}

	const Texture2D &texture = this->tileSet->getTexture();

	ClearBackground(RAYWHITE);
	if (IsTextureValid(texture)) {
		Vector2 origin = Vector2{0, 0};
		Vector2 tileSize = tileSet->getTileSize();
		Vector2 atlasSizeInTiles = {
			static_cast<float>(tileSet->getTexture().width) / tileSize.x,
			static_cast<float>(tileSet->getTexture().height) / tileSize.y,
		};

		// Draw texture itself
		DrawTextureEx(this->tileSet->getTexture(), origin, 0.0f,
					  RPGPP_DRAW_MULTIPLIER, WHITE);

		// Draw a grid
		for (int x = 0; x < atlasSizeInTiles.x; x++) {
			for (int y = 0; y < atlasSizeInTiles.y; y++) {
				Rectangle tileRect = {tileSize.x * x * RPGPP_DRAW_MULTIPLIER,
									  tileSize.y * y * RPGPP_DRAW_MULTIPLIER,
									  tileSize.x * RPGPP_DRAW_MULTIPLIER,
									  tileSize.y * RPGPP_DRAW_MULTIPLIER};
				DrawRectangleLinesEx(tileRect, 1.0f, Fade(GRAY, 0.5f));

				if (CheckCollisionPointRec(mouseWorldPos, tileRect)) {
					DrawRectangleLinesEx(tileRect, 1.0f, Fade(GRAY, 0.5));
				}
			}
		}
	} else
		DrawText("Texture Load Failure...", 0, 0, 20, RED);

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}
