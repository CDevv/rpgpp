#include "tileSetTextureView.h"

void TileSetTextureViewer::drawCanvas() {
    ClearBackground(RAYWHITE);
    if (const Texture2D& texture = this->tileSet->getTexture(); IsTextureValid(texture))
        DrawTexture(this->tileSet->getTexture(), 0, 0, WHITE);
    else
        DrawText("Texture Load Failure...", 0, 0, 20, RED);
}
