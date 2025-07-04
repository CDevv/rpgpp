#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <memory>
#include <raylib.h>
#include <string>
#include "windowContainer.hpp"
#include "resourceViewerBox.hpp"
#include "worldViewBox.hpp"
#include "panelView.hpp"

class MouseInputComponent;

class EditorInterfaceService {
private:
    Font uiFont;
    WindowContainer windowContainer;
    std::unique_ptr<WorldViewBox> worldView;
    std::unique_ptr<PanelView> panelView;
    ResourceViewerBox resourceView;
    Vector2 mousePos;
    Vector2 hoverPos;
    int chosenTileSize;
    bool chosenTileSizeEditMode;
public:
    EditorInterfaceService();
    ~EditorInterfaceService();
    void setInitial();
    void update();
    void draw();
    Font getFont();
    void drawTooltip(Rectangle rect, std::string text);
    WindowContainer& getWindowContainer();
};

#endif
