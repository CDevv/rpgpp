#include "game.hpp"
#include <stdexcept>

Game *Game::instance_ = nullptr;
std::unique_ptr<StateService> Game::state = std::unique_ptr<StateService>{};
std::unique_ptr<WorldService> Game::world = std::unique_ptr<WorldService>{};
std::unique_ptr<InterfaceService> Game::ui = std::unique_ptr<InterfaceService>{};

Game::Game()
{
    if (instance_ == nullptr) {
        instance_ = this;
    } else {
        throw std::runtime_error("Instance of Game already exists.");
    }
}

Game& Game::instance()
{
    if (instance_ == nullptr) {
        throw std::runtime_error("There is no instance of Game.");
    }

    return *instance_;
}

void Game::init()
{
    state = std::make_unique<StateService>();
    world = std::make_unique<WorldService>();
    ui = std::make_unique<InterfaceService>();
}

StateService& Game::getState()
{
    return *state;
}

WorldService& Game::getWorld()
{
    return *world;
}

InterfaceService& Game::getUi()
{
    return *ui;
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
