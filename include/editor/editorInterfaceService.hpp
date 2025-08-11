#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <memory>
#include <raylib.h>
#include <string>
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
    ProjectMenuBox projectMenu;
    TabList tabList;
    WindowContainer windowContainer;
    std::unique_ptr<WorldViewBox> worldView;
    std::unique_ptr<PanelView> panelView;
    ResourceViewerBox resourceView;
    Vector2 mousePos;
    Vector2 hoverPos;
    bool mouseLock;
public:
    EditorInterfaceService();
    ~EditorInterfaceService();
    void setInitial();
    void unload();
    void update();
    void draw();
    Font getFont();
    void drawTooltip(Rectangle rect, std::string text);
    void setMouseLock(bool value);
    bool getMouseLock();
    TabList& getTabList();
    WindowContainer& getWindowContainer();
};

#endif
