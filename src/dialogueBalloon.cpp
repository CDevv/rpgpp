#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "interfaceService.hpp"

DialogueBalloon::DialogueBalloon() {}

DialogueBalloon::DialogueBalloon(Rectangle rect)
{
	this->rect = rect;
}

void DialogueBalloon::draw()
{
	Font font = Game::getUi().getFont();
	Texture uiTexture = Game::getUi().getTexture();

	Vector2 origin = { 0.0f, 0.0f };
	NPatchInfo info = NPatchInfo { 
        Rectangle { 0, 0, static_cast<float>(uiTexture.width), static_cast<float>(uiTexture.height) },
        3 * 3, 3 * 3, uiTexture.width - (3 * 3), uiTexture.height - (3 * 3)
    };

    DrawTextureNPatch(uiTexture, info, rect, origin, 0.0f, WHITE);

    DrawTextEx(font, "Hello.", Vector2 { rect.x + 9, rect.y + 9 }, 13 * 3, 1, WHITE);
}