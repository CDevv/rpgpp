#include "editor.hpp"

Editor *Editor::instance_ = nullptr;
EditorInterfaceService *Editor::ui = nullptr;
FileSystemService *Editor::fs = nullptr;

Editor::Editor()
{
    if (instance_ != nullptr) {

    } else {
        instance_ = this;
    }
}

void Editor::init()
{
    ui = new EditorInterfaceService;
    fs = new FileSystemService;
}

void Editor::update()
{
    ui->update();
}

void Editor::draw()
{
    ui->draw();
}

void Editor::unload()
{
    ui->unload();
    delete ui;

    fs->unload();
    delete fs;
}

EditorInterfaceService *Editor::getUi()
{
    return ui;
}

FileSystemService *Editor::getFileSystem()
{
    return fs;
}


