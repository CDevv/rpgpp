#ifndef _RPGPP_LUA_STATEAPI_H
#define _RPGPP_LUA_STATEAPI_H

#include "sol/forward.hpp"
#include "sol/state_view.hpp"
#include "stateService.hpp"

void lua_gamestate_setval(const std::string &prop, sol::object value);
Value lua_gamestate_getval(const std::string &prop);
void lua_gamestate_set(sol::state_view lua);

#endif
