#include "uiElement.hpp"

#include <memory>

#include "nlohmann/json_fwd.hpp"

UIElement::UIElement() { props = std::make_unique<nlohmann::json>(nlohmann::json::object()); };

void UIElement::setProperties(const nlohmann::json &newProps) {
	props.reset();
	props = std::make_unique<nlohmann::json>(newProps);
}

nlohmann::json &UIElement::getProperties() { return *props; }

void UIElement::draw() {}
void UIElement::update() {}
