#include "game.hpp"

Game *Game::instance_ = nullptr;
StateService *Game::state = nullptr;
WorldService *Game::world = nullptr;
InterfaceService *Game::ui = nullptr;

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
    world = new WorldService;
    ui = new InterfaceService;
}

StateService *Game::getState()
{
    return state;
}

WorldService * Game::getWorld()
{
    return world;
}

InterfaceService *Game::getUi()
{
    return ui;
}

void Game::update()
{
    world->update();
    ui->update();
}

void Game::draw()
{
    world->draw();
    ui->draw();
}

void Game::unload()
{
    world->unload();
    ui->unload();
}
