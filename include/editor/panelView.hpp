#ifndef _RPGPP_EDITOR_PANELVIEW_H
#define _RPGPP_EDITOR_PANELVIEW_H

#include <raylib.h>
#include "tileSetPanelView.hpp"
#include "mapPanelView.hpp"

class PanelView {
private:
    Vector2 pos;
    Rectangle rect;
    TileSetPanelView tileSetPanel;
    MapPanelView mapPanel;
public:
    PanelView();
    PanelView(Rectangle rect);
    void update();
    void draw();
};

#endif
