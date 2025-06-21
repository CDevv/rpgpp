#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include <memory>
#include <raylib.h>
#include <string>
#include "tileset.hpp"
#include "mouseInput.hpp"
#include "worldViewBox.hpp"
#include "propertiesBox.hpp"
#include "panelView.hpp"

class MouseInputComponent;

class EditorInterfaceService {
private:
    Font uiFont;
    std::unique_ptr<WorldViewBox> worldView;
    std::unique_ptr<PanelView> panelView;
    PropertiesBox propertiesBox;
    Vector2 mousePos;
    Vector2 hoverPos;
    int chosenTileSize;
    bool chosenTileSizeEditMode;
public:
    EditorInterfaceService();
    ~EditorInterfaceService();
    void update();
    void draw();
    void unload();
    Font getFont();
    void drawTooltip(Rectangle rect, std::string text);
};

#endif
