#include "uiElement.hpp"

#include <functional>
#include <memory>
#include <string>

#include "game.hpp"
#include "gamedata.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"

UIElement::UIElement() : UIElement(INTERFACE_NULL) {
	for (int i = 0; i < RPGPP_CALLBACK_TYPE_MAX; i++) {
		CallbackType idx = static_cast<CallbackType>(i);
		callbacks[idx] = [] {};
	}
}

UIElement::UIElement(InterfaceElementType elementType) { this->elementType = elementType; }

InterfaceElementType UIElement::getType() { return elementType; }

void UIElement::draw() {}
void UIElement::update() {}

void UIElement::onNotify(Event event) {
	if (event.key == KEY_UP) {
		Game::getUi().getCurrentView()->changeFocusedElement(upButton.title);
	}
	if (event.key == KEY_DOWN) {
		Game::getUi().getCurrentView()->changeFocusedElement(downButton.title);
	}
	if (event.key == KEY_LEFT) {
		Game::getUi().getCurrentView()->changeFocusedElement(leftButton.title);
	}
	if (event.key == KEY_RIGHT) {
		Game::getUi().getCurrentView()->changeFocusedElement(rightButton.title);
	}
	if (event.key == KEY_Z) {
		callbacks[CALLBACK_TRIGGER]();
	}
}

bool UIElement::isFocusable() { return focusable; }

void UIElement::invokeCallback(CallbackType type) { callbacks[type](); }

void UIElement::setCallback(CallbackType type, std::function<void()> callback) { callbacks[type] = callback; }

void UIElement::setUpElement(const std::string &title) { upButton.title = title; }

void UIElement::setDownElement(const std::string &title) { downButton.title = title; }

void UIElement::setLeftElement(const std::string &title) { leftButton.title = title; }

void UIElement::setRightElement(const std::string &title) { rightButton.title = title; }

nlohmann::json UIElement::dumpJson() {
	auto j = nlohmann::json::object();
	j["upButton"] = upButton;
	j["downButton"] = downButton;
	j["leftButton"] = leftButton;
	j["rightButton"] = rightButton;
	return j;
}

void UIElement::fromJson(const nlohmann::json &json) {
	upButton = json.at("upButton");
	downButton = json.at("downButton");
	leftButton = json.at("leftButton");
	rightButton = json.at("rightButton");
}

UIElementBin UIElement::dumpBin() {
	UIElementBin bin;
	bin.props["upButton"] = upButton;
	bin.props["downButton"] = downButton;
	bin.props["leftButton"] = leftButton;
	bin.props["rightButton"] = rightButton;
	return bin;
}

void UIElement::fromBin(UIElementBin &bin) {
	upButton = std::get<UIElementRef>(bin.props["upButton"]);
	downButton = std::get<UIElementRef>(bin.props["downButton"]);
	leftButton = std::get<UIElementRef>(bin.props["leftButton"]);
	rightButton = std::get<UIElementRef>(bin.props["rightButton"]);
}

std::map<std::string, xxx::any_ptr> UIElement::getProps() {
	auto map = std::map<std::string, xxx::any_ptr>{};
	map["upButton"] = &upButton;
	map["downButton"] = &downButton;
	map["leftButton"] = &leftButton;
	map["rightButton"] = &rightButton;
	return map;
}
