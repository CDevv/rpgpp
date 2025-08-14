#ifndef _RPGPP_EDITOR_PANELVIEW_H
#define _RPGPP_EDITOR_PANELVIEW_H

#include <raylib.h>
#include "tileSetPanelView.hpp"
#include "mapPanelView.hpp"
#include "actorPanelView.hpp"

class PanelView {
private:
    Vector2 pos;
    Rectangle rect;
    TileSetPanelView tileSetPanel;
    MapPanelView mapPanel;
    ActorPanelView actorPanel;
public:
    PanelView();
    PanelView(Rectangle rect);
    void setInitial();
    void update();
    void draw();
};

#endif
