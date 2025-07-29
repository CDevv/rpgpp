#include "game.hpp"
#include "gamedata.hpp"
#include <cstdio>
#include <raylib.h>
#include <sol/forward.hpp>
#include <sol/state_view.hpp>
#include <sol/types.hpp>
#include <stdexcept>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}
#include <sol/sol.hpp>

Game *Game::instance_ = nullptr;
std::unique_ptr<GameData> Game::gameData = std::unique_ptr<GameData>{};
bool Game::usesBin = false;
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
    gameData = std::make_unique<GameData>();
    usesBin = false;
    state = std::make_unique<StateService>();
    world = std::make_unique<WorldService>();
    ui = std::make_unique<InterfaceService>();
}

void Game::useBin(std::string filePath)
{
    gameData = std::make_unique<GameData>(deserializeFile(filePath));
    usesBin = true;

    SetWindowTitle(gameData->title.c_str());

    world->setRoomBin(gameData->rooms.at(0));
}

GameData& Game::getBin()
{
    return *gameData;
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

void printer()
{
    printf("man\n");
}

void draw_text_lua(const char* text, int posX, int posY, int fontSize)
{
    DrawText(text, posX, posY, fontSize, BLACK);
}

void clear_bg_lua()
{
    ClearBackground(RAYWHITE);
}

extern "C" int luaopen_lib(lua_State* L)
{
    sol::state_view lua(L);
    lua.open_libraries(sol::lib::base);
    lua.set_function("printer", &printer);

    lua.set_function("init_window", &InitWindow);
    lua.set_function("close_window", &CloseWindow);
    lua.set_function("window_should_close", &WindowShouldClose);
    lua.set_function("begin_drawing", &BeginDrawing);
    lua.set_function("end_drawing", &EndDrawing);
    lua.set_function("set_fps", &SetTargetFPS);
    lua.set_function("clear_background", &clear_bg_lua);
    lua.set_function("draw_text", draw_text_lua);

    return (0);
}
