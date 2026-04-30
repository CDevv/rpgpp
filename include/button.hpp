#ifndef _RPGPP_BUTTON_H
#define _RPGPP_BUTTON_H

#include "colorRect.hpp"
#include "gamedata.hpp"
#include "label.hpp"
#include "raylib.h"
#include "uiElement.hpp"

class Button : public UIElement {
public:
	Label label;
	ColorRect colorRect;
	Color normalTextColor;
	Color focusedTextColor;
	Color shownTextColor;

	Button();

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setRect(const Rectangle &rect);
	Rectangle getRect();
	void setText(const std::string &text);

	void update() override;
	void draw() override;
};

#endif