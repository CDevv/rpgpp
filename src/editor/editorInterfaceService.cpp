#include "editorInterfaceService.hpp"
#include <cstdio>
#include <exception>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <imgui_internal.h>
#include "fileSystemService.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "nfd.h"
#include "windows/projectBinaryViewWindow.hpp"
#include "resourceViewerBox.hpp"
#include "windowContainer.hpp"

#include <sol/sol.hpp>
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
    demoGuiActive = false;

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

    Texture2D closeTexture = LoadTexture("close.png");
    this->closeTexture = closeTexture;

    this->logoTexture = LoadTexture("logo.png");

    mousePos = Vector2 { 0, 0 };
    hoverPos = Vector2 { 0, 0 };
    mouseLock = false;
    mouseBoxLayer = VIEWBOX_LAYER_BASE;

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
    mainView.update();
    tabList.update();
    panelView->update();
    resourceView.update();

    windowContainer.update();
}

void EditorInterfaceService::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    bool openedAboutWIndow = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Project..")) {
                fs.promptOpenProject();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem("Open Project..")) {
                fs.promptOpenProject();
            }
            if (ImGui::MenuItem("Open Binary..")) {
                ProjectBinaryViewWindow& projectBinary = windowContainer.openProjectBinaryView();
                FS_Result fsResult = fs.openGameData();
                if (fsResult.result == NFD_OKAY) {
                    GameData gameData = deserializeFile(fsResult.path);
                    projectBinary.setData(gameData);
                }
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Dev")) {
            ImGui::MenuItem("ImGui Demo", nullptr, &demoGuiActive);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About RPG++")) {
                openedAboutWIndow = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGuiErrorRecoveryState state;
    ImGui::ErrorRecoveryStoreState(&state);

    if (!fs.projectIsOpen()) {
        drawMainView();
    } else {
        drawProjectView();
    }

    windowContainer.drawProjectBinaryView();

    if (demoGuiActive) {
        ImGui::ShowDemoWindow();
    }

    if (openedAboutWIndow) {
        windowContainer.openAbout();
        openedAboutWIndow = false;
    }

    windowContainer.drawAbout();
}

void EditorInterfaceService::drawMainView()
{
    mainView.setRect(Rectangle {
        0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())
    });
    mainView.draw();
}

void EditorInterfaceService::drawProjectView()
{
    FileSystemService& fs = Editor::getFileSystem();

    Rectangle projectMenuRect = Rectangle { 0, 16, static_cast<float>(GetScreenWidth()), 36 };
    projectMenu.setRect(projectMenuRect);
    projectMenu.draw();

    if (fs.projectIsOpen()) {
        tabList.setRect(Rectangle {
            static_cast<float>(GetScreenWidth() * 0.2) + 8, 36 + 16 + 4, static_cast<float>(GetScreenWidth() * 0.8) - (8 * 2), 18
        });
        tabList.draw();
    }

    panelView->setRect(Rectangle {
        static_cast<float>(GetScreenWidth() * 0.2) + 4, (36 + 24 + 18 + 4),
        static_cast<float>(GetScreenWidth() * 0.8) - 8, static_cast<float>(GetScreenHeight() - (36 + 24 + 18 + 4  + 4))
    });
    panelView->draw();

    resourceView.setRect(Rectangle {
        0, 36 + 18, static_cast<float>(GetScreenWidth() * 0.2), static_cast<float>(GetScreenHeight() - (38 + 18 + 4))
    });
    resourceView.draw();
}

Font EditorInterfaceService::getFont()
{
    return uiFont;
}

Texture2D EditorInterfaceService::getCloseTexture()
{
    return closeTexture;
}

Texture2D EditorInterfaceService::getLogoTexture()
{
    return logoTexture;
}

WindowContainer& EditorInterfaceService::getWindowContainer()
{
    return windowContainer;
}

TabList& EditorInterfaceService::getTabList()
{
    return tabList;
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
