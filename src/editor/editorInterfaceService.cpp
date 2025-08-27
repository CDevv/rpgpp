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

    Rectangle projectMenuRect = Rectangle { 0, 16, static_cast<float>(GetScreenWidth()), 36 };
    projectMenu = ProjectMenuBox(projectMenuRect);

    float elementBaseY = projectMenuRect.y + projectMenuRect.height;

    Rectangle propRect = Rectangle { 4, elementBaseY + 4, 160, static_cast<float>(GetScreenHeight() - (elementBaseY + 8)) };
    resourceView = ResourceViewerBox(propRect);

    Rectangle tabListRect = Rectangle
    {
        propRect.x + propRect.width + 4, propRect.y,
        static_cast<float>(GetScreenWidth() - propRect.width - 12), 24
    };
    tabList = TabList(tabListRect);

    Rectangle windowRect = Rectangle
    {
        propRect.width + 8, elementBaseY + 4 + tabListRect.height + 4,
        static_cast<float>(GetScreenWidth() - (propRect.width + 16) + 4), static_cast<float>(GetScreenHeight() - (elementBaseY + 8) - (tabListRect.height + 4))
    };
    panelView = std::make_unique<PanelView>(windowRect);

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
    if (fs.projectIsOpen()) {
        std::filesystem::path fPath = std::string(fs.getProject()->getProjectBasePath()).append("/").append("run.lua");
        std::filesystem::remove(fPath);

        #ifdef __linux__
        fs.getProject()->cleanCompilation();
        #endif
    }
}

void EditorInterfaceService::update()
{
    tabList.update();
    panelView->update();
    resourceView.update();
}

void EditorInterfaceService::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    Rectangle appMenuRect = Rectangle
    {
        0, 0, static_cast<float>(GetScreenWidth()), 16
    };
    DrawRectangleRec(appMenuRect, DARKGRAY);

    projectMenu.draw();

    if (fs.projectIsOpen()) {
        tabList.draw();
    }
    panelView->draw();

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

void EditorInterfaceService::setMouseBoxLayer(ViewBoxLayer boxLayer)
{
    this->mouseBoxLayer = boxLayer;
}

ViewBoxLayer EditorInterfaceService::getMouseBoxLayer()
{
    return this->mouseBoxLayer;
}
