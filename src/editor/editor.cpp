#include "editor.hpp"

Editor *Editor::instance_ = nullptr;
std::unique_ptr<EditorInterfaceService> Editor::ui = std::unique_ptr<EditorInterfaceService>{};
std::unique_ptr<FileSystemService> Editor::fs = std::unique_ptr<FileSystemService>{};

Editor::Editor()
{
    if (instance_ != nullptr) {

    } else {
        instance_ = this;
    }
}

void Editor::init()
{
    ui = std::make_unique<EditorInterfaceService>();
    fs = std::make_unique<FileSystemService>();
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
}

EditorInterfaceService& Editor::getUi()
{
    return *ui;
}

FileSystemService& Editor::getFileSystem()
{
    return *fs;
}


