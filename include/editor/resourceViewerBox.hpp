#ifndef _RPGPP_EDITOR_RESOURCEVIEWERBOX_H
#define _RPGPP_EDITOR_RESOURCEVIEWERBOX_H

#include <raylib.h>
#include <string>

class ResourceViewerBox {
private:
    Rectangle rect;
    bool dropdownEditMode;
    int dropdownActive;
    bool deleteConfirmOpen;
    std::string deleteConfirmPath;
    void drawContextMenu(std::string resPath);
    void drawTileSets();
    void drawMaps();
    void drawActors();
    std::string errorMessage;
public:
    ResourceViewerBox();
    ResourceViewerBox(Rectangle rect);
    void setRect(Rectangle rect);
    void update();
    void draw();
};

#endif
