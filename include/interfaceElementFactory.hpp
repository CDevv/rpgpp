#ifndef _RPGPP_INTERFACEELEMENTFACTORY_H
#define _RPGPP_INTERFACEELEMENTFACTORY_H

#include <array>
#include <functional>
#include <memory>

#include "uiElement.hpp"

std::unique_ptr<UIElement> constructElement(InterfaceElementType type);

#endif