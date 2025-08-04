#include "game.hpp"
#include "gamedata.hpp"
#include <cstdio>
#include <raylib.h>
#include <sol/sol.hpp>
#include <sol/forward.hpp>
#include <sol/state_view.hpp>
#include <sol/types.hpp>

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
extern "C" 
#ifdef _WIN32
__declspec(dllexport) 
#endif
int luaopen_rpgpplua(lua_State* L)
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

    sol::usertype<Game> game_type = lua.new_usertype<Game>("game",
        sol::constructors<Game()>());

    game_type["init"] = &Game::init;
    game_type["use_bin"] = &Game::useBin;
    game_type["update"] = &Game::update;
    game_type["draw"] = &Game::draw;
    game_type["unload"] = &Game::unload;

    return (0);
}