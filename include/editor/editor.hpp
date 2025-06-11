#ifndef _RPGPP_EDITOR_H
#define _RPGPP_EDITOR_H

#include "editorInterfaceService.hpp"

class Editor {
private:
    static Editor *instance_;
    static EditorInterfaceService *ui;
public:
    Editor();
    static EditorInterfaceService *getUi();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
