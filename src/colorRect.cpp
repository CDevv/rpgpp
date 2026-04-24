#include "colorRect.hpp"

#include <raylib.h>

#include <vector>

#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"

ColorRect::ColorRect() : ColorRect({1, 1, 1, 1}) {}

ColorRect::ColorRect(const Rectangle rect) {
	this->rect = rect;
	this->color = RAYWHITE;
}

std::map<std::string, xxx::any_ptr> ColorRect::getProps() { return {{"rect", &rect}, {"color", &color}}; }

void ColorRect::fromJson(const nlohmann::json &json) {
	this->rect = json.at("rect");
	this->color = json.at("color");
}

nlohmann::json ColorRect::dumpJson() {
	auto j = nlohmann::json::object();
	j["rect"] = rect;
	j["color"] = color;
	return j;
}

void ColorRect::fromBin(UIElementBin &bin) {
	this->rect = std::get<Rectangle>(bin.props["rect"]);
	this->color = std::get<Color>(bin.props["color"]);
}

UIElementBin ColorRect::dumpBin() {
	UIElementBin bin;
	bin.props["rect"] = rect;
	bin.props["color"] = color;
	return bin;
}

void ColorRect::setColor(const Color newColor) { this->color = newColor; }

void ColorRect::update() {
	// TODO
}

void ColorRect::draw() { DrawRectangleRec(rect, color); }
