#include "game.hpp"

Game *Game::instance_ = nullptr;
StateService *Game::state = nullptr;

Game::Game() {}

Game& Game::instance()
{
    if (instance_ == nullptr) {
        instance_ = new Game();
    }

    return *instance_;
}

void Game::init()
{
    state = new StateService;
}

StateService *Game::getState()
{
    return state;
}

void Game::update()
{
    state->getRoom()->update();
}

void Game::draw()
{
    state->getRoom()->draw();
}

void Game::unload()
{
    state->getRoom()->unload();
}
