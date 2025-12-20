#include "editorInterfaceService.hpp"
#include <cstdio>
#include <cstring>
#include <raylib.h>
#include <imgui.h>
#include <imgui_internal.h>
#include "edui.hpp"
#include "fileSystemService.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "nfd.h"
#include "windows/projectBinaryViewWindow.hpp"
#include "resourceViewerBox.hpp"
#include "windowContainer.hpp"

#include <filesystem>

#ifdef _WIN32
#include "winapi.hpp"
#endif

EditorInterfaceService::EditorInterfaceService()
{
    demoGuiActive = false;

    //appearance
    appearance.border = BLACK;
    appearance.primaryColor = Color { 30, 30, 30, 255 };
    appearance.secondaryColor = Color { 35, 35, 35, 255 };
    appearance.fontSize = 22;
    appearance.textColor = WHITE;

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

    test_str = std::make_unique<char>();
    strcpy(test_str.get(), "Hi!");
}

EditorInterfaceService::~EditorInterfaceService()
{
    //UnloadFont(uiFont);
}

void EditorInterfaceService::setInitial()
{
    panelView->setInitial();
}

void EditorInterfaceService::unload()
{
    UnloadFont(uiFont);
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
    //update gui state
    state.mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state.mouseDown = 1;
    } else {
        state.mouseDown = 0;
    }


    state.keyEntered = GetKeyPressed();
    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        state.keyMod = KEY_LEFT_SHIFT;
    }
    if (IsKeyDown(KEY_BACKSPACE)) {
        state.keyDown = KEY_BACKSPACE;
    }
    if (IsKeyDown(KEY_LEFT)) {
        state.keyDown = KEY_LEFT;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        state.keyDown = KEY_RIGHT;
    }

    state.keyChar = GetCharPressed();

    //mainView.update();
    //tabList.update();
    //panelView->update();
    //resourceView.update();

    //windowContainer.update();
}

void EditorInterfaceService::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    bool openedAboutWIndow = false;

    if (EdUi::Button(1, Rectangle { 0, 50, 200, 50 }, "Blep")) {
        printf("hello! \n");
    }
    if (EdUi::Button(2, Rectangle { 200, 50, 200, 50 }, "Blep2")) {
        printf("hello 2! \n");
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            printf("%s \n", fsResult.absolutePath.c_str());
        }

    }

    EdUi::TextField(3, Rectangle { 0, 120, 200, 26 }, test_str.get(), 256);

    EdUi::End();

    //Texture cross = Editor::getResources().getTexture("cross");
    //EdUi::IconButton(Rectangle { 200, 50, 50, 50 }, cross);

    /*
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
        windowContainer.open("About");
        openedAboutWIndow = false;
    }

    windowContainer.drawWindow("About");
    */
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

EdUi::Appearance& EditorInterfaceService::getAppearance()
{
    return appearance;
}

EdUi::UiState& EditorInterfaceService::getState()
{
    return state;
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
