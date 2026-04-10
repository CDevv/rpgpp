#ifndef _RPGPP_IMAGERECT_H
#define _RPGPP_IMAGERECT_H

#include <raylib.h>

#include <string>

#include "uiElement.hpp"

class ImageRect : public UIElement {
private:
	Rectangle rect;
	std::string source;
	Texture2D texture;

public:
	ImageRect();
	ImageRect(Rectangle rect);
	void setSource(const std::string &source);
	void setTexture(Texture2D texture);
	void update();
	void draw();
};

#endif
