#ifndef _RPGPP_COLORRECT_H
#define _RPGPP_COLORRECT_H

#include <raylib.h>

#include "uiElement.hpp"

class ColorRect : public UIElement {
public:
	Rectangle rect;
	Color color;

	ColorRect();
	explicit ColorRect(Rectangle rect);

	void setColor(Color newColor);
	void update() override;
	void draw() override;
};

#endif
