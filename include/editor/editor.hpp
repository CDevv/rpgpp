#ifndef _RPGPP_EDITOR_H
#define _RPGPP_EDITOR_H

#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"

class Editor {
private:
    static Editor *instance_;
    static EditorInterfaceService *ui;
    static FileSystemService *fs;
public:
    Editor();
    static EditorInterfaceService *getUi();
    static FileSystemService *getFileSystem();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
