#include "textArea.hpp"
#include "game.hpp"
#include <raylib.h>

TextArea::TextArea() : rect(Rectangle {})
{
}

TextArea::TextArea(Rectangle rect)
{
    this->rect = rect;
    this->content = "";
}

void TextArea::update()
{

}

void TextArea::draw()
{
    Vector2 textPos = Vector2 { 0, 0 };
    Vector2 charMeasure = Vector2 { 0, 0 };
    for (int i = 0; i < content.size(); i++) {
        putChar(i, &textPos, &charMeasure);
    }
}

void TextArea::putChar(int i, Vector2 *charPos, Vector2 *charMeasure) const
{
    charPos->x += charMeasure->x;

    DrawTextPro(Game::getUi().getFont(), TextSubtext(content.c_str(), i, 1),
        *charPos, Vector2 { 0, 0 }, 0.0f, 13 * 3, 1, BLACK);

    *charMeasure = MeasureTextEx(Game::getUi().getFont(),
			    TextSubtext(content.c_str(), i, 1), 13 * 3, 1.0f);
}

void TextArea::setText(const std::string &text)
{
    this->content = text;
}
