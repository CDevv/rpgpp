#ifndef _RPGPP_EDITOR_MAPPANELVIEW_H
#define _RPGPP_EDITOR_MAPPANELVIEW_H

#include <memory>
#include <raylib.h>
#include "worldViewBox.hpp"

class MapPanelView {
private:
    Rectangle rect;
    std::unique_ptr<WorldViewBox> worldView;
    std::unique_ptr<WorldViewBox> tileSetView;
public:
    MapPanelView();
    MapPanelView(Rectangle rect);
    void update();
    void draw();
};

#endif
