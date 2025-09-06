#include "projectInitWindow.hpp"
#include <raygui.h>
#include "project.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include <memory>

ProjectInitWindow::ProjectInitWindow() {}

ProjectInitWindow::ProjectInitWindow(Rectangle rect)
{
	this->rect = rect;
	this->active = false;

	this->titleEditMode = false;
    this->titleText = "";
    this->hasSetDirPath = false;
    this->dirPath = "";

    this->title = std::make_unique<char>();
    *title = '\0';
}

void ProjectInitWindow::setActive()
{
	active = true;
}

void ProjectInitWindow::closeWindow()
{
	EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
    ui.setMouseLock(false);

    dirPath = "";
    hasSetDirPath = false;
    titleText = "";
    *title = '\0';
    active = false;
}

void ProjectInitWindow::draw()
{
	if (active) {
		FileSystemService& fs = Editor::getFileSystem();
        EditorInterfaceService& ui = Editor::getUi();

        if (GuiWindowBox(rect, "New Project..")) {
            closeWindow();
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 40, rect.width - 16, 24 }, "Title..");
        if (GuiTextBox(Rectangle { rect.x + 8, rect.y + 64, rect.width - 16, 24 }, title.get(), 13, titleEditMode)) {
            titleEditMode = !titleEditMode;
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 96, rect.width - 16, 24 }, "Directory..");
        Rectangle dirPathLabelRect = Rectangle { rect.x + 8, rect.y + 120, rect.width - (16 + 24), 24 };
        if (hasSetDirPath) {
            GuiLabel(dirPathLabelRect, dirPath.c_str());
        } else {
            GuiLabel(dirPathLabelRect, "Not set..");
        }
        if (GuiButton(Rectangle { rect.x + 8 + (rect.width - (16 + 24)), rect.y + 120, 24, 24 }, GuiIconText(ICON_FOLDER_OPEN, NULL))) {
            FS_Result fsResult = fs.openFolder();
            if (fsResult.result == NFD_OKAY) {
                dirPath = fsResult.path;
                hasSetDirPath = true;
            }
        }

        if (GuiButton(Rectangle { rect.x + 184, rect.y + (rect.height - (24 + 8)), 120, 24 }, "Submit..")) {
        	titleText = title.get();
            
            if (titleText.empty()) return;
            if (!hasSetDirPath) return;

            Project::generateNewProj(titleText, dirPath);

            fs.setProject(TextFormat("%s/%s/proj.rpgpp", dirPath.c_str(), titleText.c_str()));
            
            closeWindow();
        }

        if (active) {
        	if (CheckCollisionPointRec(GetMousePosition(), rect)) {
                ui.setMouseBoxLayer(VIEWBOX_LAYER_WINDOW);
                ui.setMouseLock(true);
            } else {
                ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
                ui.setMouseLock(false);
            }
        }
	}
}