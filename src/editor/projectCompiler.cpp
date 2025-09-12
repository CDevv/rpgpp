#include "projectCompiler.hpp"
#include <filesystem>

#ifdef _WIN32
#include "winapi.hpp"
#endif

#include <reproc++/run.hpp>

ProjectCompiler::ProjectCompiler() {}

void ProjectCompiler::generateCmdScript(std::string projectTitle)
{
	#ifdef _WIN32
    //Generate a .cmd file on Windows
    VsInfo vsInfo = WinVsWhere(std::string(GetApplicationDirectory()).append("vswhere.exe"));
    printf("%s \n", vsInfo.auxiliaryPath.c_str());

    std::string scriptSource;
    std::string callVcBat = "call ";
    callVcBat.append("\"").append(vsInfo.auxiliaryPath).append("\\vcvars64.bat").append("\"");

    std::string clLine = "cl /nologo /c /EHs /I ";
    clLine.append("\"").append(GetApplicationDirectory()).append("\include").append("\"")
    .append(" /I ./include /I \"").append(GetApplicationDirectory()).append("game-src\\include\" main.cpp");

    std::string appDir = std::string(GetApplicationDirectory()).substr(0, TextLength(GetApplicationDirectory()) - 1);

    std::string exeName = TextFormat("%s.exe", projectTitle.c_str());
    std::string linkLine = "link ";
    linkLine.append("/NOLOGO /LIBPATH:\"").append(GetApplicationDirectory()).append("\lib").append("\"")
    .append(" /LIBPATH:").append("\"").append(appDir).append("\"")
    .append(" /LIBPATH:\"").append(appDir).append("\\game-src\\lib\"")
    .append(" /OUT:").append(exeName)
    .append(" raylib.lib rpgpp.lib opengl32.lib gdi32.lib shell32.lib user32.lib winmm.lib")
    .append(" main.obj");

    scriptSource.append(callVcBat).append("\n\n").append(clLine).append("\n").append(linkLine).append("\n");
    printf("%s \n", scriptSource.c_str());

    SaveFileText("build.cmd", const_cast<char*>(scriptSource.data()));
    #else
    //Linux sh file
    std::string shellString = TextFormat(R"(g++ -I"%sinclude" -I"%sgame-src/include" main.cpp -o %s -L"%sgame-src/lib" -lraylib -lrpgpp)", 
        GetApplicationDirectory(), GetApplicationDirectory(), projectTitle.c_str(), GetApplicationDirectory());
    printf("%s \n", shellString.c_str());
    SaveFileText("build.sh", const_cast<char*>(shellString.data()));
    #endif
}

void ProjectCompiler::callCompiler()
{
	std::string cppCode = R"(#include <raylib.h>
#include "game.hpp"

int main()
{
    InitWindow(640, 480, "RPG++ Game.");

    Game game;
    game.init();

    Game::useBin("game.bin");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.draw();
        EndDrawing();
    }

    game.unload();
    CloseWindow();

    return 0;
}
    )";
    SaveFileText("main.cpp", const_cast<char*>(cppCode.data()));

    #ifdef _WIN32
    std::string cmdLine = TextFormat("C:\\Windows\\System32\\cmd.exe /c \"%s\"", "build.cmd");
    WinCreateProc(cmdLine);
    #else
    std::vector<std::string> rargs;
    rargs.push_back("sh");
    rargs.push_back("build.sh");

    reproc::options options;
    options.redirect.parent = true;

    reproc::process p;
    p.start(rargs, options);
    #endif
}

void ProjectCompiler::cleanCompilation(std::string projectBasePath)
{
	#ifdef _WIN32
    std::filesystem::remove(std::string(TextFormat("%s\\build.cmd", projectBasePath.c_str())));
    std::filesystem::remove(std::string(TextFormat("%s\\main.cpp", projectBasePath.c_str())));
    std::filesystem::remove(std::string(TextFormat("%s\\main.obj", projectBasePath.c_str())));
    #else
    std::filesystem::remove(std::string(TextFormat("%s/build.sh", projectBasePath.c_str())));
    std::filesystem::remove(std::string(TextFormat("%s/main.cpp", projectBasePath.c_str())));
    #endif
}