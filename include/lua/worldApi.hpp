#ifndef _RPGPP_LUA_WORLDAPI_H
#define _RPGPP_LUA_WORLDAPI_H

#include "sol/state_view.hpp"

sol::object lua_world_getroom(sol::this_state lua);
void lua_world_setroom(const std::string &room);
sol::object lua_world_getplayer(sol::this_state lua);
void lua_world_set(sol::state_view lua);

#endif