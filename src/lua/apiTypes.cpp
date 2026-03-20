#include "lua/apiTypes.hpp"
#include "player.hpp"
#include "raylib.h"
#include "sol/raii.hpp"
#include "sol/types.hpp"

void lua_types_set(sol::state_view lua) {
	lua.new_usertype<Vector2>(
		"Vector2",
		sol::factories([]() { return Vector2(); },
					   [](float a, float b) { return Vector2{a, b}; }),
		"x", &Vector2::x, "y", &Vector2::y);
	lua.new_usertype<Room>(sol::no_construction(), "GetPlayer",
						   &Room::getPlayer);
	lua.new_usertype<Player>(
		sol::no_construction(), "GetPosition", &Player::getPosition,
		"SetPosition", &Player::setPosition, "GetTilePosition",
		&Player::getTilePosition, "SetTilePosition", &Player::setTilePosition,
		"MoveByVelocity", &Player::moveByVelocity);
}