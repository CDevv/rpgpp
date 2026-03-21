#include "lua/apiTypes.hpp"
#include "actor.hpp"
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

	lua.new_enum("Direction", "DOWN_IDLE", Direction::RPGPP_DOWN_IDLE, "DOWN",
				 Direction::RPGPP_DOWN, "UP_IDLE", Direction::RPGPP_UP_IDLE,
				 "UP", Direction::RPGPP_UP, "LEFT_IDLE",
				 Direction::RPGPP_LEFT_IDLE, "LEFT", Direction::RPGPP_LEFT,
				 "RIGHT_IDLE", Direction::RPGPP_RIGHT_IDLE, "RIGHT",
				 Direction::RPGPP_RIGHT);

	lua.new_usertype<Room>(sol::no_construction(), "GetPlayer",
						   &Room::getPlayer);

	lua.new_usertype<Actor>(
		sol::no_construction(), "GetPosition", &Actor::getPosition,
		"SetPosition", &Actor::setPosition, "GetTilePosition",
		&Actor::getTilePosition, "SetTilePosition", &Actor::setTilePosition,
		"MoveByVelocity", &Actor::moveByVelocity, "ChangeAnimation",
		&Actor::changeAnimation, "PlayAnimation", &Actor::playAnimation);

	lua.new_usertype<Player>(
		sol::no_construction(), "GetPosition", &Player::getPosition,
		"SetPosition", &Player::setPosition, "GetTilePosition",
		&Player::getTilePosition, "SetTilePosition", &Player::setTilePosition,
		"MoveByVelocity", &Player::moveByVelocity, "GetActor",
		&Player::getActor);
}