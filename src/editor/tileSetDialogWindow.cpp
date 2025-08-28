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

	Rectangle tileSetViewRect = Rectangle {
		rect.x + 4, rect.y + 28, rect.width - 84, rect.height - 34
	};
	this->tileSetView = std::make_unique<WorldViewBox>(tileSetViewRect, FILE_TILESET, VIEWBOX_LAYER_DIALOG);
	this->tileSetView->enableTileSelection();
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
	this->tileSetView->update();

	if (active) {
		FileSystemService& fs = Editor::getFileSystem();
        EditorInterfaceService& ui = Editor::getUi();

		if (GuiWindowBox(rect, "Actor TileSet..")) {
			closeWindow();
		}

		this->tileSetView->draw();
		if (GuiButton(Rectangle { rect.x + (rect.width - 80) + 4, rect.y + 28, 72, 24 }, "Save")) {
			if (getSelectedTile().x != -1) {
				callback(getSelectedTile());
			}
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

void TileSetDialogWindow::setTileSet(TileSet* tileSet)
{
	this->tileSetView->setTileSet(tileSet);
}

void TileSetDialogWindow::setSelectedTile(Vector2 atlasTile)
{
	this->tileSetView->setSelectedTile(atlasTile);
}

Vector2 TileSetDialogWindow::getSelectedTile()
{
	return this->tileSetView->getSelectedTile();
}

void TileSetDialogWindow::setCallback(std::function<void(Vector2)> func)
{
	this->callback = func;
}