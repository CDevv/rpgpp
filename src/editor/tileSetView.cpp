#include "tileSetView.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "tileset.hpp"

TileSetView::Ptr TileSetView::create(TileSet *tileSet) {
	auto tileSetTextureViewer = std::make_shared<TileSetView>();
	tileSetTextureViewer->tileSet = tileSet;
	return tileSetTextureViewer;
}

void TileSetView::drawCanvas() {
	const Texture2D &texture = this->tileSet->getTexture();

	ClearBackground(RAYWHITE);
	if (IsTextureValid(texture)) {
		Vector2 origin = Vector2{0, 0};
		Vector2 tileSize = tileSet->getTileSize();
		Vector2 atlasSizeInTiles = {
			static_cast<float>(tileSet->getTexture().width) / tileSize.x,
			static_cast<float>(tileSet->getTexture().height) / tileSize.y,
		};

		for (int x = 0; x < atlasSizeInTiles.x; x++) {
			for (int y = 0; y < atlasSizeInTiles.y; y++) {
				Rectangle tileRect = {tileSize.x * x * RPGPP_DRAW_MULTIPLIER,
									  tileSize.y * y * RPGPP_DRAW_MULTIPLIER,
									  tileSize.x * RPGPP_DRAW_MULTIPLIER,
									  tileSize.y * RPGPP_DRAW_MULTIPLIER};
				DrawRectangleLinesEx(tileRect, 1.0f, RED);
			}
		}

		DrawTextureEx(this->tileSet->getTexture(), origin, 0.0f,
					  RPGPP_DRAW_MULTIPLIER, WHITE);
	} else
		DrawText("Texture Load Failure...", 0, 0, 20, RED);

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}
