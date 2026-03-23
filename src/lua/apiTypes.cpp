#include "lua/apiTypes.hpp"
#include "actor.hpp"
#include "actorContainer.hpp"
#include "collisionsContainer.hpp"
#include "interactable.hpp"
#include "interactablesContainer.hpp"
#include "player.hpp"
#include "prop.hpp"
#include "propsContainer.hpp"
#include "raylib.h"
#include "sol/forward.hpp"
#include "sol/raii.hpp"
#include "tilemap.hpp"
#include <cstdio>
#include <string>

void lua_interactable_setprop(Interactable *inter, sol::object key,
							  sol::object value) {
	if (inter == nullptr)
		return;

	auto &props = inter->getProps();
	if (!key.is<std::string>()) {
		return;
	}

	printf("%s \n", props.dump().c_str());

	const std::string keyStr = key.as<std::string>();

	nlohmann::json &target = props[keyStr];

	if (props.count(keyStr) > 0) {
		if (props[keyStr].is_object()) {
			target = props[keyStr]["value"];
		}
	}

	if (value.is<std::string>()) {
		target = value.as<std::string>();
	} else if (value.is<int>()) {
		target = value.as<int>();
	} else if (value.is<float>()) {
		target = value.as<float>();
	} else if (value.is<bool>()) {
		target = value.as<bool>();
	}
}

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

	lua.new_usertype<CollisionsContainer>(
		sol::no_construction(), "Push", &CollisionsContainer::pushObjectVec2,
		"Remove", &CollisionsContainer::removeObjectVec2, "Exists",
		&CollisionsContainer::objectExistsAtPositionVec2);

	lua.new_usertype<InteractablesContainer>(
		sol::no_construction(), "Push",
		&InteractablesContainer::addBinFromTypename, "Remove",
		&InteractablesContainer::removeObjectVec2, "Exists",
		&InteractablesContainer::objectExistsAtPositionVec2, "GetAt",
		&InteractablesContainer::getIntVec2);

	lua.new_usertype<Interactable>(
		sol::no_construction(), "SetProp", &lua_interactable_setprop,
		"IsOnTouch", &Interactable::isOnTouch, "SetOnTouch",
		&Interactable::setOnTouch, "GetPosition", &Interactable::getWorldPos,
		"GetType", &Interactable::getType, "SetType", &Interactable::setType);

	lua.new_usertype<PropsContainer>(
		sol::no_construction(), "Push", &PropsContainer::addProp, "Remove",
		&PropsContainer::removeObjectVec2, "Exists",
		&PropsContainer::objectExistsAtPositionVec2, "GetAt",
		&PropsContainer::getPropAt);

	lua.new_usertype<Prop>(
		sol::no_construction(), "GetPosition", &Prop::getWorldPos,
		"SetPosition", &Prop::setWorldPos, "GetTilePosition",
		&Prop::getWorldTilePos, "SetTilePosition", &Prop::setWorldTilePos,
		"GetInteractable", &Prop::getInteractable);

	lua.new_usertype<ActorContainer>(
		sol::no_construction(), "Push", &ActorContainer::addActor, "Remove",
		&ActorContainer::removeActor, "Exists", &ActorContainer::actorExists,
		"Get", &ActorContainer::getActor);

	lua.new_usertype<Room>(
		sol::no_construction(), "GetPlayer", &Room::getPlayer, "GetStartTile",
		&Room::getStartTile, "GetTileMap", &Room::getTileMap, "GetCollisions",
		&Room::getCollisions, "GetInteractables", &Room::getInteractables,
		"GetProps", &Room::getProps, "GetActors", &Room::getActors);

	lua.new_usertype<TileMap>(sol::no_construction(), "SetTile",
							  &TileMap::setTile, "SetEmptyTile",
							  &TileMap::setEmptyTile, "GetWorldSizeInTiles",
							  &TileMap::getMaxWorldSize);

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