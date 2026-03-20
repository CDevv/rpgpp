#include "lua/worldApi.hpp"
#include "game.hpp"
#include "sol/object.hpp"
#include "sol/table.hpp"
#include "sol/types.hpp"

sol::object lua_world_getroom(sol::this_state lua) {
	return sol::make_object(lua, &Game::getWorld().getRoom());
}

void lua_world_set(sol::state_view lua) {
	auto space = lua["World"].get_or_create<sol::table>();

	space.set_function("GetRoom", lua_world_getroom);
}