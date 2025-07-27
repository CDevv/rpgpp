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
        if (GuiButton(Rectangle { (138 + 120*6 + 8*2), 8, 120, 24 }, "Export to binary file")) {
            serializeDataToFile(binFile, project->generateStruct());
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
