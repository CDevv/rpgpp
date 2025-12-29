#include "game.hpp"
#include "gamedata.hpp"
#include "soundService.hpp"
#include <cstdio>
#include <memory>
#include <raylib.h>
#include <sol/sol.hpp>
#include <sol/forward.hpp>
#include <sol/state_view.hpp>
#include <sol/types.hpp>
#include <stdexcept>

Game *Game::instance_ = nullptr;
std::unique_ptr<GameData> Game::gameData = std::unique_ptr<GameData>{};
bool Game::usesBin = false;
std::unique_ptr<StateService> Game::state = std::unique_ptr<StateService>{};
std::unique_ptr<WorldService> Game::world = std::unique_ptr<WorldService>{};
std::unique_ptr<InterfaceService> Game::ui = std::unique_ptr<InterfaceService>{};
std::unique_ptr<ResourceService> Game::resources = std::unique_ptr<ResourceService>{};
std::unique_ptr<SoundService> Game::sounds = std::unique_ptr<SoundService>{};

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
    gameData = std::make_unique<GameData>();
    usesBin = false;
    resources = std::make_unique<ResourceService>();
    state = std::make_unique<StateService>();
    world = std::make_unique<WorldService>();
    ui = std::make_unique<InterfaceService>();
    sounds = std::make_unique<SoundService>();
}

void Game::useBin(std::string filePath)
{
    gameData = std::make_unique<GameData>(deserializeFile(filePath));
    usesBin = true;

    for (auto item : gameData->images) {
        Image image = LoadImageFromMemory(".png", item.second.data.data(), item.second.dataSize);
        Texture2D texture = LoadTextureFromImage(image);
        resources->addTexture(item.first, texture);
    }

    SetWindowTitle(gameData->title.c_str());

    world->setRoomBin(gameData->rooms.at(0));
}

GameData& Game::getBin()
{
    return *gameData;
}

ResourceService& Game::getResources()
{
    return *resources;
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

SoundService& Game::getSounds()
{
    return *sounds;
}

void Game::update()
{
    sounds->update();
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
    sounds->unload();
    world->unload();
    ui->unload();
}
