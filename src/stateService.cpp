#include "stateService.hpp"

StateService::StateService()
{
    gameState.emplace("test", false);
}

bool StateService::getProp(std::string prop)
{
    return gameState.at(prop);
}

void StateService::unload()
{

}

