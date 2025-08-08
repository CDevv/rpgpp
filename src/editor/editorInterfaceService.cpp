#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "gamedata.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "editor.hpp"
#include "resourceViewerBox.hpp"
#include "windowContainer.hpp"
#include "tabButton.hpp"

#include <sol/sol.hpp>
#include "game.hpp"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <filesystem>
#include <reproc++/run.hpp>

#ifdef _WIN32
#include "winapi.hpp"
#endif

EditorInterfaceService::EditorInterfaceService()
{
    //get codepoints
    std::vector<int> codepoints;
    //ASCII latin
    for (int i = 0; i < 92; i++) {
        codepoints.push_back(i + 32);
    }
    //cyrillic
    for (int i = 0x400; i <= 0x4FF; i++) {
        codepoints.push_back(i);
    }

    //load the font
    uiFont = LoadFontEx("resources/LanaPixel.ttf", 13, codepoints.data(), codepoints.size());

    GuiLoadStyle("rpgpp.rgs");
    GuiLoadIcons("iconset.rgi", true);
    GuiSetFont(uiFont);

    mousePos = Vector2 { 0, 0 };
    hoverPos = Vector2 { 0, 0 };
    mouseLock = false;

    Rectangle tabListRect = Rectangle
    {
        (138 + 120 + 8), 8,
        120*5, 24
    };
    tabList = TabList(tabListRect);

    Rectangle windowRect = Rectangle
    {
        176, 48,
        static_cast<float>(GetScreenWidth() - 386), static_cast<float>(GetScreenHeight() - 56)
    };
    Rectangle renderRect = Rectangle {
        (windowRect.x + 2), (windowRect.y + 24),
        (windowRect.width - 4), (windowRect.height - 30)
    };
    panelView = std::make_unique<PanelView>(windowRect);

    Rectangle propRect = Rectangle { 8, 48, 160, static_cast<float>(GetScreenHeight() - 56) };
    resourceView = ResourceViewerBox(propRect);

    windowContainer = WindowContainer();
}

EditorInterfaceService::~EditorInterfaceService()
{
    UnloadFont(uiFont);
}

void EditorInterfaceService::setInitial()
{
    panelView->setInitial();
}

void EditorInterfaceService::unload()
{
    FileSystemService& fs = Editor::getFileSystem();
    std::filesystem::path fPath = std::string(fs.getProject()->getProjectBasePath()).append("/").append("run.lua");
    std::filesystem::remove(fPath);
}

void EditorInterfaceService::update()
{
    tabList.update();
    panelView->update();
    resourceView.update();
}

void EditorInterfaceService::draw()
{
    tabList.draw();

    panelView->draw();

    FileSystemService& fs = Editor::getFileSystem();
    if (GuiButton(Rectangle { 8, 8, 120, 24 }, "Open..")) {
        fs.promptOpenProject();
        panelView->setInitial();
    }

    if (fs.fileIsOpen()) {
        if (GuiButton(Rectangle { 138, 8, 120, 24 }, "Save")) {
            if (fs.getType() == FILE_TILESET) {
                TileSet *tileSet = fs.getTileSet();

                std::string jsonString = tileSet->dumpJson().dump(4);

                char *text = const_cast<char*>(jsonString.data());
                SaveFileText(fs.getOpenedFilePath().c_str(), text);
            } else {
                std::string mapJsonString = fs.getRoom()->dumpJson().dump(4);

                char *text = const_cast<char*>(mapJsonString.data());
                SaveFileText(fs.getOpenedFilePath().c_str(), text);
            }
        }  
    }
    if (fs.projectIsOpen()) {
        Project* project = fs.getProject();
        std::string binFile = std::string(project->getProjectBasePath());
        binFile.append("/game.bin");
		
		Rectangle exportRect = Rectangle {
			(138 + 120*6 + 8*2), 8, 120, 24
		};
        if (GuiButton(exportRect, "Export to binary file")) {
            serializeDataToFile(binFile, project->generateStruct());
        }
		Rectangle runRect = exportRect;
        runRect.width = runRect.height;
		runRect.x += exportRect.width * 2 + 16;
		if (GuiButton(runRect, GuiIconText(ICON_PLAYER_PLAY, NULL))) {
            //Save game.bin first
            std::string binFile = std::string(fs.getProject()->getProjectBasePath()).append("/game.bin");
            serializeDataToFile(binFile, fs.getProject()->generateStruct());

			std::string luaCodeString = R"(
	printer()

			init_window(640, 480, "lraylib")

			g = game.new()
			g:init()

			game.use_bin("game.bin")

			set_fps(60)

			while not window_should_close() do
				g:update()
				begin_drawing()
				clear_background()
				g:draw()
				end_drawing()
			end

			close_window()
			)";
			SaveFileText("run.lua", const_cast<char*>(luaCodeString.data()));

            #ifdef _WIN32
            std::string fromLuaLib = TextFormat("%s\\game-src\\lib\\rpgpplua.dll", GetApplicationDirectory());
            std::string toLuaLib = TextFormat("%s\\rpgpplua.dll", fs.getProject()->getProjectBasePath().c_str());
            std::filesystem::copy(fromLuaLib, toLuaLib);
            #endif
			
			reproc::options options;
			options.redirect.parent = true;

			std::vector<std::string> rargs;
			#ifdef _WIN32
				rargs.push_back(std::string("\"").append(GetApplicationDirectory()).append("lua.exe").append("\""));
			#else
				rargs.push_back(std::string(GetApplicationDirectory()).append("lua"));
			#endif
			
			rargs.push_back("-lrpgpplua");
			
			#ifdef _WIN32
				rargs.push_back(std::string("\"").append(fs.getProject()->getProjectBasePath()).append("\\").append("run.lua").append("\""));
			#else
				rargs.push_back(std::string(fs.getProject()->getProjectBasePath()).append("/").append("run.lua"));
			#endif
			printf("%s \n", rargs[0].c_str());
			printf("%s \n", rargs[1].c_str());
			printf("%s \n", rargs[2].c_str());
			reproc::process p;
			p.start(rargs, options);
			
			#ifdef _WIN32
			std::string cmdLine = std::string(rargs[0]).append(" ").append(rargs[1]).append(" ").append(rargs[2]);
			printf("%s \n", cmdLine.c_str());
			WinCreateProc(cmdLine);

            //Clean up
            std::filesystem::remove(toLuaLib);
            //std::filesystem::remove(std::string(TextFormat("%s\\run.lua", GetWorkingDirectory())));
			#endif
		}

        Rectangle buildRect = runRect;
        buildRect.x += runRect.width + 8;
        if (GuiButton(buildRect, GuiIconText(220, NULL))) {
            #ifdef _WIN32
            std::string vswherePath = std::string("\"").append(GetApplicationDirectory()).append("vswhere.exe");
			//WinVsWhere(vswherePath);
            fs.getProject()->compileProject();
			#endif
        }
    }

    if (GuiButton(Rectangle { (138 + 120*6 + 8*2 + 120), 8, 120, 24 }, "Open binary file..")) {
        FS_Result fsResult = fs.openGameData();
        GameData data = deserializeFile(fsResult.path);
        printf("%s\n", data.title.c_str());
        ProjectBinaryViewWindow& window = windowContainer.openProjectBinaryView();
        window.setData(data);
    }

    resourceView.draw();
    windowContainer.draw();

    GuiUnlock();
}

Font EditorInterfaceService::getFont()
{
    return uiFont;
}

WindowContainer& EditorInterfaceService::getWindowContainer()
{
    return windowContainer;
}

TabList& EditorInterfaceService::getTabList()
{
    return tabList;
}

void EditorInterfaceService::drawTooltip(Rectangle rect, std::string text)
{
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        DrawRectangleLinesEx(rect, 1.0f, GRAY);

        Vector2 mousePos = Vector2Add(GetMousePosition(), Vector2 { 16, 0 });
        Vector2 textPos = Vector2Add(mousePos, Vector2 { 4, 4 });
        Vector2 textSize = MeasureTextEx(uiFont, text.c_str(), 13, 1);
        GuiPanel(
            Rectangle {
                mousePos.x, mousePos.y,
                textSize.x + 8, textSize.y + 8
            }, NULL);
        DrawTextEx(uiFont, text.c_str(), textPos, 13, 1, GRAY);
    }
}

void EditorInterfaceService::setMouseLock(bool value)
{
    this->mouseLock = value;
}

bool EditorInterfaceService::getMouseLock()
{
    return this->mouseLock;
}
