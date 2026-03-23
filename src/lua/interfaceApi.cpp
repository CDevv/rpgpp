#include "lua/interfaceApi.hpp"
#include "game.hpp"
#include "sol/table.hpp"

void lua_opendiag(const std::string &id) { Game::getUi().showDialogue(id); }

void lua_ui_set(sol::state_view &lua) {
	auto space = lua["Interface"].get_or_create<sol::table>();
	space.set_function("OpenDialogue", lua_opendiag);
}
