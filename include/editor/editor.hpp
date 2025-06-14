#ifndef _RPGPP_EDITOR_H
#define _RPGPP_EDITOR_H

#include <memory>
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"

class Editor {
private:
    static Editor *instance_;
    static std::unique_ptr<EditorInterfaceService> ui;
    static std::unique_ptr<FileSystemService> fs;
public:
    Editor();
    static EditorInterfaceService& getUi();
    static FileSystemService& getFileSystem();
    void init();
    void update();
    void draw();
    void unload();
};

#endif
