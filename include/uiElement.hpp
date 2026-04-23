#ifndef _RPGPP_UIELEMENT_H
#define _RPGPP_UIELEMENT_H

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

class UIElement {
public:
	std::unique_ptr<nlohmann::json> props;

	UIElement();
	virtual ~UIElement() = default;
	virtual void update();
	virtual void draw();
	virtual nlohmann::json &getProperties();
	void setProperties(const nlohmann::json &newProps);
};

#endif
