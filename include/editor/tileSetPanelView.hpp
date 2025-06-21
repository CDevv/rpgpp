#ifndef _RPGPP_EDITOR_TILESETPANELVIEW_H
#define _RPGPP_EDITOR_TILESETPANELVIEW_H

#include <memory>
#include "worldViewBox.hpp"

class TileSetPanelView {
private:
    Rectangle rect;
    std::unique_ptr<WorldViewBox> worldView;
public:
    TileSetPanelView();
    TileSetPanelView(Rectangle rect);
    void update();
    void draw();
};

#endif
