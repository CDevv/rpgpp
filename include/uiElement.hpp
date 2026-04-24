#ifndef _RPGPP_UIELEMENT_H
#define _RPGPP_UIELEMENT_H

#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "any_ptr/any_ptr.h"
#include "gamedata.hpp"
#include "jsonConversions.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "saveable.hpp"

class UIElement : public ISaveable {
public:
	UIElement();
	virtual ~UIElement() = default;
	virtual void update();
	virtual void draw();

	// virtual nlohmann::json dumpJson();
	virtual void fromJson(const nlohmann::json &json) = 0;
	virtual void fromBin(UIElementBin &bin) = 0;
	virtual UIElementBin dumpBin() = 0;
	virtual std::map<std::string, xxx::any_ptr> getProps() = 0;
};

#endif
