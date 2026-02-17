#include "imageRect.hpp"
#include <game.hpp>
#include <raylib.h>

ImageRect::ImageRect() : rect(Rectangle {}), texture() {
}

ImageRect::ImageRect(Rectangle rect) : texture()
{
	this->rect = rect;
	this->source = "";
}

void ImageRect::setSource(const std::string &source)
{
	this->source = source;
}

void ImageRect::setTexture(Texture2D texture)
{
	this->texture = texture;
}

void ImageRect::update()
{

}

void ImageRect::draw()
{
	DrawTexturePro(texture,
	Rectangle { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
	rect, Vector2 { 0, 0 }, 0.0f, WHITE);
}
