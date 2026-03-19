#include "lua/stateApi.hpp"
#include "game.hpp"
#include "sol/table.hpp"

void lua_gamestate_setval(const std::string &prop, bool value) {
	Game::getState().setProp(prop, value);
}

bool lua_gamestate_getval(const std::string &prop) {
	return Game::getState().getProp(prop);
}

void lua_gamestate_set(sol::state_view lua) {
	auto space = lua["GameState"].get_or_create<sol::table>();

	space.set_function("Set", lua_gamestate_setval);
	space.set_function("Get", lua_gamestate_getval);
}