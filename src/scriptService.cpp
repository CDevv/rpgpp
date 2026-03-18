#include "scriptService.hpp"
#include "lua/soundsApi.hpp"
#include "sol/state_view.hpp"

#include "lua/interfaceApi.hpp"
#include "sol/table.hpp"
#include <nlohmann/json_fwd.hpp>

ScriptService::ScriptService() {
	state.open_libraries(sol::lib::base);
	setLua(state);
}

void ScriptService::setLua(sol::state_view lua) {
	lua_ui_set(lua);
	lua_sounds_set(lua);
}

sol::state &ScriptService::getState() { return state; }

void ScriptService::addToState(nlohmann::json &j) {
	auto props = state["props"].get_or_create<sol::table>();
	props.clear();

	for (auto prop : j.items()) {
		if (prop.value().is_object()) {
			props[prop.key()] = prop.value().at("value").get<std::string>();
		} else if (prop.value().is_string()) {
			props[prop.key()] = prop.value().get<std::string>();
		} else if (prop.value().is_number()) {
			props[prop.key()] = prop.value().get<float>();
		}
	}
}