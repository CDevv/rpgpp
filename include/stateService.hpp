#ifndef _RPGPP_STATESERVICE_H
#define _RPGPP_STATESERVICE_H

#include <map>
#include <string>
#include <variant>

#include "sol/table.hpp"
#include "sol/types.hpp"

/** The StateService is responsible for storing gameplay-related variables
 * that make up the state of the game. */
using Value = std::variant<sol::lua_nil_t, bool, double, std::string, sol::table, sol::function>;
class StateService {
private:
	/** A pair of string keys and boolean values. */
	std::map<std::string, Value> gameState;

public:
	/** Empty constructor */
	StateService();
	/** Set a property */
	void setProp(const std::string &prop, Value value);
	/** Get a boolean property from the container. */
	Value getProp(const std::string &prop) const;
	/** Unload routine. */
	void unload() const;
};

#endif
