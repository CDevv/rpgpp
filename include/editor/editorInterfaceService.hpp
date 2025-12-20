#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <string>
#include <memory>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include "mainView.hpp"
#include "projectMenuBox.hpp"
#include "tabList.hpp"
#include "windowContainer.hpp"
#include "resourceViewerBox.hpp"
#include "worldViewBox.hpp"
#include "panelView.hpp"

#include "edui.hpp"

class MouseInputComponent;

class EditorInterfaceService {
private:
    EdUi::Appearance appearance;
    EdUi::UiState state;
    Font uiFont;
    MainView mainView;
    ProjectMenuBox projectMenu;
    TabList tabList;
    WindowContainer windowContainer;
    std::unique_ptr<WorldViewBox> worldView;
    std::unique_ptr<PanelView> panelView;
    ResourceViewerBox resourceView;
    Vector2 mousePos;
    Vector2 hoverPos;
    bool mouseLock;
    ViewBoxLayer mouseBoxLayer;
    bool demoGuiActive;
    void drawMainView();
    void drawProjectView();
    char testStr[256];
public:
    EditorInterfaceService();
    ~EditorInterfaceService();
    void setInitial();
    void unload();
    void update();
    void draw();
    EdUi::Appearance& getAppearance();
    EdUi::UiState& getState();
    Font getFont();
    void setMouseLock(bool value);
    bool getMouseLock();
    void setMouseBoxLayer(ViewBoxLayer boxLayer);
    ViewBoxLayer getMouseBoxLayer();
    TabList& getTabList();
    WindowContainer& getWindowContainer();
};

#endif
