#ifndef _RPGPP_EDITOR_TILESETPANELVIEW_H
#define _RPGPP_EDITOR_TILESETPANELVIEW_H

#include <memory>
#include "worldViewBox.hpp"
#include "propertiesBox.hpp"

class TileSetPanelView {
private:
    Rectangle rect;
    std::unique_ptr<WorldViewBox> worldView;
    PropertiesBox propBox;
public:
    TileSetPanelView();
    TileSetPanelView(Rectangle rect);
    void setRect(Rectangle rect);
    void setInitial();
    void update();
    void draw();
};

#endif
