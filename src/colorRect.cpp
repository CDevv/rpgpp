#include "colorRect.hpp"

#include <raylib.h>

#include <vector>

#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "uiElement.hpp"

ColorRect::ColorRect() : ColorRect({1, 1, 1, 1}) {}

ColorRect::ColorRect(const Rectangle rect) : UIElement(INTERFACE_COLORRECT) {
	this->rect = rect;
	this->color = RAYWHITE;
	this->borderColor = BLACK;
	this->borderWidth = 0;
}

std::map<std::string, xxx::any_ptr> ColorRect::getProps() {
	return {{"rect", &rect}, {"color", &color}, {"borderColor", &borderColor}, {"borderWidth", &borderWidth}};
}

void ColorRect::fromJson(const nlohmann::json &json) {
	this->rect = json.at("rect");
	this->color = json.at("color");
	this->borderColor = json.at("borderColor");
	this->borderWidth = json.at("borderWidth");
}

nlohmann::json ColorRect::dumpJson() {
	auto j = nlohmann::json::object();
	j["rect"] = rect;
	j["color"] = color;
	j["borderColor"] = borderColor;
	j["borderWidth"] = borderWidth;
	return j;
}

void ColorRect::fromBin(UIElementBin &bin) {
	this->rect = std::get<Rectangle>(bin.props["rect"]);
	this->color = std::get<Color>(bin.props["color"]);
	this->borderColor = std::get<Color>(bin.props["borderColor"]);
	this->borderWidth = std::get<int>(bin.props["borderWidth"]);
}

UIElementBin ColorRect::dumpBin() {
	UIElementBin bin = UIElement::dumpBin();
	bin.props["rect"] = rect;
	bin.props["color"] = color;
	bin.props["borderColor"] = borderColor;
	bin.props["borderWidth"] = borderWidth;
	return bin;
}

void ColorRect::setColor(const Color newColor) { this->color = newColor; }

void ColorRect::update() {
	// TODO
}

void ColorRect::draw() {
	DrawRectangleRec(rect, color);
	if (borderWidth > 0) {
		DrawRectangleLinesEx(rect, static_cast<float>(borderWidth), borderColor);
	}
}
