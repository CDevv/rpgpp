#ifndef _RPGPP_IMAGERECT_H
#define _RPGPP_IMAGERECT_H

#include <raylib.h>

#include <string>

#include "uiElement.hpp"

class ImageRect : public UIElement {
private:
	Texture2D texture;

public:
	Rectangle rect;
	std::string source;

	ImageRect();
	ImageRect(Rectangle rect);

	void setSource(const std::string &source);
	void setTexture(Texture2D texture);
	void update() override;
	void draw() override;
};

#endif
