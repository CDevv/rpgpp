#include "lua/interfaceApi.hpp"
#include "game.hpp"
#include "sol/table.hpp"

void lua_opendiag(const std::string &id) {
	if (Game::getBin().dialogues.count(id) > 0) {
		auto diag = Game::getBin().dialogues[id];
		Game::getUi().showDialogue(diag);
	} else {
		fprintf(stderr, "This dialogue does not exist: %s \n", id.c_str());
	}
}

void lua_ui_set(sol::state_view &lua) {
	auto space = lua["Interface"].get_or_create<sol::table>();
	space.set_function("OpenDialogue", lua_opendiag);
}
