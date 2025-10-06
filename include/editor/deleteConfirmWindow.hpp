#ifndef _RPGPP_DELETECONFIRMWINDOW_H
#define _RPGPP_DELETECONFIRMWINDOW_H

#include <raylib.h>
#include <string>

class DeleteConfirmWindow {
private:
    bool active;
    Rectangle rect;
    std::string filePath;
public:
    DeleteConfirmWindow();
    DeleteConfirmWindow(Rectangle rect);
    void setActive();
    void closeWindow();
    void setFilePath(std::string filePath);
    void draw();
};

#endif
