#include "lua/stateApi.hpp"
#include "game.hpp"
#include "sol/forward.hpp"
#include "sol/table.hpp"
#include "sol/types.hpp"
#include "stateService.hpp"

void lua_gamestate_setval(const std::string &prop, sol::object value) {
	if (value.is<sol::nil_t>()) {
		Game::getState().setProp(prop, sol::nil);
	} else if (value.is<bool>()) {
		Game::getState().setProp(prop, value.as<bool>());
	} else if (value.is<double>()) {
		Game::getState().setProp(prop, value.as<double>());
	} else if (value.is<std::string>()) {
		Game::getState().setProp(prop, value.as<std::string>());
	} else if (value.is<sol::table>()) {
		Game::getState().setProp(prop, value.as<sol::table>());
	} else if (value.is<sol::function>()) {
		Game::getState().setProp(prop, value.as<sol::function>());
	}
}

Value lua_gamestate_getval(const std::string &prop) {
	return Game::getState().getProp(prop);
}

void lua_gamestate_set(sol::state_view lua) {
	auto space = lua["GameState"].get_or_create<sol::table>();

	space.set_function("Set", lua_gamestate_setval);
	space.set_function("Get", lua_gamestate_getval);
}
