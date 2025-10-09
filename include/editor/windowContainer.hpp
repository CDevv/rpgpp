#ifndef _RPGPP_EDITOR_WINDOWCONTAINER_H
#define _RPGPP_EDITOR_WINDOWCONTAINER_H

#include <map>
#include <memory>

#include "windowPopup.hpp"

#include "windows/projectBinaryViewWindow.hpp"
#include "windows/tileSetDialogWindow.hpp"

class WindowContainer {
private:
    bool windowOpen;
    ProjectBinaryViewWindow projectBinaryView;
    TileSetDialogWindow tileSetDialog;
    std::map<std::string, std::unique_ptr<WindowPopup>> mapW;
public:
    WindowContainer();
    bool isWindowOpen();
    void setWindowOpen(bool value);
    ProjectBinaryViewWindow& openProjectBinaryView();
    TileSetDialogWindow& openTileSetDialog();
    void update();
    void draw();
    void drawTileSetDialog();
    void drawProjectBinaryView();
    WindowPopup& open(std::string id);
    void drawWindow(std::string id);
};

#endif
