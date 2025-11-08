#ifndef _RPGPP_EDITOR_MAPPANELVIEW_H
#define _RPGPP_EDITOR_MAPPANELVIEW_H

#include <memory>
#include <raylib.h>
#include "mapToolsBox.hpp"
#include "collisionInfoPanel.hpp"
#include "interactableInfoPanel.hpp"
#include "propInfoPanel.hpp"
#include "actorInfoPanel.hpp"
#include "worldViewBox.hpp"
#include "propertiesBox.hpp"

class MapPanelView {
private:
    Rectangle rect;
    std::unique_ptr<WorldViewBox> worldView;
    MapToolsBox toolsBox;
    std::unique_ptr<WorldViewBox> tileSetView;
    CollisionInfoPanel collisionInfo;
    InteractableInfoPanel interactableInfo;
    PropInfoPanel propInfo;
    ActorInfoPanel actorInfo;
    PropertiesBox propBox;
    int actionModeToggle;
    int layoutModeToggle;
    bool layoutDropdownEditMode;
public:
    MapPanelView();
    MapPanelView(Rectangle rect);
    void setRect(Rectangle rect);
    void setInitial();
    void update();
    void draw();
};

#endif
