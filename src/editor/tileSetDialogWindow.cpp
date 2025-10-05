#include "tileSetDialogWindow.hpp"
#include <imgui.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "worldViewBox.hpp"

TileSetDialogWindow::TileSetDialogWindow() {}

TileSetDialogWindow::TileSetDialogWindow(Rectangle rect)
{
	this->boxLayer = VIEWBOX_LAYER_DIALOG;
	this->active = false;
	this->rect = rect;
	this->chosenTile = Vector2 { -1, -1 };

	Rectangle tileSetViewRect = Rectangle {
		rect.x + 8, rect.y + 8 + 16 + 24, rect.width - 84, rect.height - (34 + 24)
	};
	this->tileSetView = std::make_unique<WorldViewBox>(tileSetViewRect, FILE_TILESET, VIEWBOX_LAYER_DIALOG);
	this->tileSetView->enableTileSelection();
}

void TileSetDialogWindow::setActive()
{
	active = true;
	ImGui::OpenPopup("Actor TileSet");
	this->chosenTile = Vector2 { -1, -1 };
}

void TileSetDialogWindow::closeWindow()
{
	EditorInterfaceService& ui = Editor::getUi();
	ui.setMouseLock(false);
	ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
	active = false;
	ImGui::CloseCurrentPopup();
}

void TileSetDialogWindow::update()
{
    tileSetView->update();
}

void TileSetDialogWindow::draw()
{
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::BeginPopupModal("Actor TileSet", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        tileSetView->draw();

        if (ImGui::Button("Save")) {
            if (getSelectedTile().x != -1) {
                callback(getSelectedTile());
            }
            closeWindow();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            closeWindow();
        }

        ImGui::EndPopup();
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
