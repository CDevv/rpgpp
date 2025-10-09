#ifndef _RPGPP_EDITOR_WINDOWPOPUP_H
#define _RPGPP_EDITOR_WINDOWPOPUP_H

#include <string>
#include <raylib.h>

class WindowPopup {
protected:
    Rectangle rect;
    bool propExists;
    std::string prop;
public:
    std::string imPopupId;
    virtual ~WindowPopup() {};
    WindowPopup();
    WindowPopup(std::string imPopupId, Rectangle rect);
    void setProp(std::string prop);
    std::string getProp();
    bool hasProp();
    void openWindow();
    void closeWindow();
    virtual void update();
    virtual void draw() {};
};

#endif
