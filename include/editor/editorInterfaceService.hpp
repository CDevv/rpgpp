#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

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

class MouseInputComponent;

class EditorInterfaceService {
private:
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
public:
    EditorInterfaceService();
    ~EditorInterfaceService();
    void setInitial();
    void unload();
    void update();
    void draw();
    Font getFont();
    void setMouseLock(bool value);
    bool getMouseLock();
    void setMouseBoxLayer(ViewBoxLayer boxLayer);
    ViewBoxLayer getMouseBoxLayer();
    TabList& getTabList();
    WindowContainer& getWindowContainer();
};

#endif
