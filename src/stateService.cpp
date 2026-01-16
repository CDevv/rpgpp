#include "stateService.hpp"

StateService::StateService() { gameState.emplace("test", false); }

bool StateService::getProp(const std::string &prop) const {
  return gameState.at(prop);
}

void StateService::unload() const {}
