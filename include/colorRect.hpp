#ifndef _RPGPP_COLORRECT_H
#define _RPGPP_COLORRECT_H

#include "uiElement.hpp"
#include <raylib.h>

class ColorRect : public UIElement {
  private:
	Rectangle rect;
	Color color;

  public:
	ColorRect();

	explicit ColorRect(Rectangle rect);
	void setColor(Color newColor);
	void update() override;
	void draw() override;
};

#endif
