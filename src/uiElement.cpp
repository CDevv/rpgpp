#include "uiElement.hpp"

#include <memory>

#include "nlohmann/json_fwd.hpp"

UIElement::UIElement() = default;

void UIElement::fromJson(const nlohmann::json &json) {}

void fromBin(UIElementBin &bin) {}
UIElementBin dumpBin() {}
std::map<std::string, xxx::any_ptr> getProps() {}

void UIElement::draw() {}
void UIElement::update() {}
