#ifndef _RPGPP_EDITOR_RESOURCEVIEWERBOX_H
#define _RPGPP_EDITOR_RESOURCEVIEWERBOX_H

#include <raylib.h>

class ResourceViewerBox {
private:
    Rectangle rect;
    bool dropdownEditMode;
    int dropdownActive;
    void drawTileSets();
    void drawMaps();
public:
    ResourceViewerBox();
    ResourceViewerBox(Rectangle rect);
    void update();
    void draw();
};

#endif