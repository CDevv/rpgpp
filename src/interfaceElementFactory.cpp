#include "interfaceElementFactory.hpp"

#include <memory>

#include "button.hpp"
#include "colorRect.hpp"
#include "dialogueArea.hpp"
#include "imageRect.hpp"
#include "label.hpp"
#include "ninePatchImageRect.hpp"
#include "textArea.hpp"
#include "uiElement.hpp"

static std::array<std::function<std::unique_ptr<UIElement>()>, RPGPP_INTERFACE_ELEMENT_MAX> interfaceFactory = {
	[] { return std::make_unique<UIElement>(); }, [] { return std::make_unique<TextArea>(); },
	[] { return std::make_unique<Label>(); },		[] { return std::make_unique<ColorRect>(); },
	[] { return std::make_unique<ImageRect>(); }, [] { return std::make_unique<NinePatchImageRect>(); },
	[] { return std::make_unique<Button>(); },	   [] { return std::make_unique<DialogueArea>(); }};

std::unique_ptr<UIElement> constructElement(InterfaceElementType type) { return interfaceFactory[type](); }