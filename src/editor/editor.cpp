#include "editor.hpp"

#include <memory>

#include "resourceService.hpp"

Editor *Editor::instance_ = nullptr;
std::unique_ptr<EditorInterfaceService> Editor::ui = std::unique_ptr<EditorInterfaceService>{};
std::unique_ptr<FileSystemService> Editor::fs = std::unique_ptr<FileSystemService>{};
std::unique_ptr<ResourceService> Editor::resources = std::unique_ptr<ResourceService>{};

Editor::Editor()
{
    if (instance_ != nullptr) {

    } else {
        instance_ = this;
    }
}

void Editor::init()
{
    resources = std::make_unique<ResourceService>();
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
    ui->unload();
    resources->unload();
}

EditorInterfaceService& Editor::getUi()
{
    return *ui;
}

FileSystemService& Editor::getFileSystem()
{
    return *fs;
}

ResourceService& Editor::getResources()
{
    return *resources;
}
