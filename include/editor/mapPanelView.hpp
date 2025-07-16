#ifndef _RPGPP_EDITOR_MAPPANELVIEW_H
#define _RPGPP_EDITOR_MAPPANELVIEW_H

#include <memory>
#include <raylib.h>
#include "collisionInfoPanel.hpp"
#include "interactableInfoPanel.hpp"
#include "worldViewBox.hpp"
#include "propertiesBox.hpp"

class MapPanelView {
private:
    Rectangle rect;
    std::unique_ptr<WorldViewBox> worldView;
    std::unique_ptr<WorldViewBox> tileSetView;
    CollisionInfoPanel collisionInfo;
    InteractableInfoPanel interactableInfo;
    PropertiesBox propBox;
    int actionModeToggle;
    int layoutModeToggle;
    bool layoutDropdownEditMode;
public:
    MapPanelView();
    MapPanelView(Rectangle rect);
    void setInitial();
    void update();
    void draw();
};

#endif
