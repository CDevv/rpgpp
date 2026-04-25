#include "button.hpp"

#include <cstdio>

#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "uiElement.hpp"

Button::Button() : UIElement(INTERFACE_BUTTON) {
	focusable = true;
	normalTextColor = label.textColor;
	focusedTextColor = DARKGRAY;
	shownTextColor = label.textColor;
	callbacks[CALLBACK_TRIGGER] = [] { printf("button trigger.. \n"); };
	callbacks[CALLBACK_UNFOCUSED] = [this] { shownTextColor = normalTextColor; };
	callbacks[CALLBACK_FOCUSED] = [this] { shownTextColor = focusedTextColor; };
}

void Button::fromJson(const nlohmann::json &json) {
	UIElement::fromJson(json);
	colorRect.fromJson(json);
	label.fromJson(json);
	normalTextColor = label.textColor;
	focusedTextColor = json["focusedTextColor"];
}

nlohmann::json Button::dumpJson() {
	auto j = UIElement::dumpJson();
	auto colorRectDump = colorRect.dumpJson();
	for (auto &i : colorRectDump.items()) {
		j[i.key()] = i.value();
	}
	auto labelDump = label.dumpJson();
	for (auto &i : labelDump.items()) {
		j[i.key()] = i.value();
	}
	j["focusedTextColor"] = focusedTextColor;
	return j;
}

void Button::fromBin(UIElementBin &bin) {
	UIElement::fromBin(bin);
	colorRect.fromBin(bin);
	label.fromBin(bin);
	focusedTextColor = std::get<Color>(bin.props["focusedTextColor"]);
}

UIElementBin Button::dumpBin() {
	auto bin = UIElement::dumpBin();
	bin.props.merge(colorRect.dumpBin().props);
	bin.props.merge(label.dumpBin().props);
	bin.props["focusedTextColor"] = focusedTextColor;
	return bin;
}

std::map<std::string, xxx::any_ptr> Button::getProps() {
	auto map = UIElement::getProps();
	map.merge(colorRect.getProps());
	map.merge(label.getProps());
	map["focusedTextColor"] = &focusedTextColor;
	return map;
}

void Button::setRect(const Rectangle &rect) {
	this->label.rect = rect;
	this->colorRect.rect = rect;
}

void Button::setText(const std::string &text) { this->label.setText(text); }

void Button::update() {
	label.textColor = shownTextColor;
	colorRect.update();
	label.update();
}

void Button::draw() {
	colorRect.draw();
	label.draw();
}
