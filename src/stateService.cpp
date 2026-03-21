#include "stateService.hpp"
#include "sol/error.hpp"
#include <stdexcept>

StateService::StateService() { gameState.emplace("test", false); }

void StateService::setProp(const std::string &prop, Value value) {
	gameState[prop] = value;
}

Value StateService::getProp(const std::string &prop) const {
	if (gameState.count(prop) == 0) {
		throw sol::error("This prop does not exist");
	}
	return gameState.at(prop);
}

void StateService::unload() const {}
