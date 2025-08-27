#include "tileSetDialogWindow.hpp"
#include <raygui.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"

TileSetDialogWindow::TileSetDialogWindow() {}

TileSetDialogWindow::TileSetDialogWindow(Rectangle rect)
{
	this->boxLayer = VIEWBOX_LAYER_DIALOG;
	this->active = false;
	this->rect = rect;
	this->chosenTile = Vector2 { -1, -1 };
}

void TileSetDialogWindow::setActive()
{
	active = true;
	this->chosenTile = Vector2 { -1, -1 };
}

void TileSetDialogWindow::closeWindow()
{
	EditorInterfaceService& ui = Editor::getUi();
	ui.setMouseLock(false);
	ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
	active = false;
}

void TileSetDialogWindow::update()
{

}

void TileSetDialogWindow::draw()
{
	if (active) {
		FileSystemService& fs = Editor::getFileSystem();
        EditorInterfaceService& ui = Editor::getUi();

		if (GuiWindowBox(rect, "Actor TileSet..")) {
			closeWindow();
		}

		if (active) {
            if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            	ui.setMouseBoxLayer(boxLayer);
                ui.setMouseLock(true);
            } else {
            	ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
                ui.setMouseLock(false);
            }
        }
	}
}