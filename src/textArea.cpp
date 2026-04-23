#include "textArea.hpp"

#include <raylib.h>

#include "game.hpp"

TextArea::TextArea() : rect(Rectangle{}) {}

TextArea::TextArea(Rectangle rect) {
	this->rect = rect;
	this->content = "";

	props->push_back({"rect_x", rect.x});
	props->push_back({"rect_y", rect.y});
	props->push_back({"rect_width", rect.width});
	props->push_back({"rect_height", rect.height});

	props->push_back({"content", content});
}

void TextArea::update() {
	// TODO
	content = props->at("content");
}

void TextArea::draw() {
	auto textPos = Vector2{0, 0};
	auto charMeasure = Vector2{0, 0};
	for (int i = 0; i < content.size(); i++) {
		putChar(i, &textPos, &charMeasure);
	}
}

void TextArea::putChar(int i, Vector2 *charPos, Vector2 *charMeasure) const {
	charPos->x += charMeasure->x;

	DrawTextPro(Game::getUi().getFont(), TextSubtext(content.c_str(), i, 1), *charPos, Vector2{0, 0}, 0.0f, 13 * 3, 1,
				BLACK);

	*charMeasure = MeasureTextEx(Game::getUi().getFont(), TextSubtext(content.c_str(), i, 1), 13 * 3, 1.0f);
}

void TextArea::setText(const std::string &text) {
	this->content = text;
	props->at("content") = text;
}
