#include "lua/apiTypes.hpp"
#include "sol/raii.hpp"

void lua_types_set(sol::state_view lua) {
	lua.new_usertype<lua_Vector2>(
		"Vector2",
		sol::constructors<lua_Vector2(), lua_Vector2(float, float)>(), "x",
		&lua_Vector2::x, "y", &lua_Vector2::y);
}