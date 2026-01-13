#ifndef _RPGPP_EDITOR_WINDOWPOPUP_H
#define _RPGPP_EDITOR_WINDOWPOPUP_H

#include <string>
#include <raylib.h>
#include <TGUI/AllWidgets.hpp>

class WindowPopup {
protected:
    tgui::ChildWindow::Ptr childWindow;
    Rectangle rect;
    bool propExists;
    std::string prop;
public:
    std::string imPopupId;
    virtual ~WindowPopup() {};
    WindowPopup();
    WindowPopup(const std::string &imPopupId, Rectangle rect);
    void setProp(const std::string &prop);
    std::string getProp();
    bool hasProp();
    void openWindow();
    void closeWindow();
    virtual void update();
    virtual void draw() {};
};

#endif
