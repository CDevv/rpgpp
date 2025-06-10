#include "editor.hpp"

Editor *Editor::instance_ = nullptr;
EditorInterfaceService *Editor::ui = nullptr;

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
}
