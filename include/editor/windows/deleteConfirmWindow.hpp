#ifndef _RPGPP_DELETECONFIRMWINDOW_H
#define _RPGPP_DELETECONFIRMWINDOW_H

#include <raylib.h>
#include "windowPopup.hpp"

class DeleteConfirmWindow : public WindowPopup {
private:
    Rectangle rect;
public:
    DeleteConfirmWindow();
    DeleteConfirmWindow(Rectangle rect);
    void openWindow();
    void closeWindow();
    void draw();
};

#endif
