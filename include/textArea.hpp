#ifndef _RPGPP_TEXTAREA_H
#define _RPGPP_TEXTAREA_H

#include <raylib.h>

#include <string>

#include "uiElement.hpp"

class TextArea : public UIElement {
private:
	void putChar(int i, Vector2 *charPos, Vector2 *charMeasure) const;

public:
	Rectangle rect;
	std::string content;

	TextArea();
	TextArea(Rectangle rect);

	void setText(const std::string &text);
	void update() override;
	void draw() override;
};

#endif
