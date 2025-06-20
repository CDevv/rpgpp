#include "tileSetPropertiesBox.hpp"
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

TileSetPropertiesBox::TileSetPropertiesBox()
{
    chosenTileSize = 0;
    chosenTileSizeEditMode = false;
}

void TileSetPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileSet *tileSet = fs.getTileSet();
    chosenTileSize = tileSet->getTileSize();
}

void TileSetPropertiesBox::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    TileSet *tileSet = fs.getTileSet();
    this->tileSet = tileSet;

    if (chosenTileSize >= 16) {
        tileSet->setTileSize(chosenTileSize);
    }
}

void TileSetPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileSet *tileSet = fs.getTileSet();
    this->tileSet = tileSet;

    GuiPanel(Rectangle { 8, 48, 160, static_cast<float>(GetScreenHeight() - 56) }, "TileSet Props");

    GuiLabel(Rectangle { 16, 80, 144, 24 }, "TILE SIZE");
    if (GuiValueBox(Rectangle { 16, 112, 144, 24 }, NULL, &chosenTileSize, 16, 32, chosenTileSizeEditMode)) {
        chosenTileSizeEditMode = !chosenTileSizeEditMode;
    }

    GuiLabel(Rectangle { 16, 144, 144, 24 }, "TEXTURE SOURCE");
    std::string sourceFileName = GetFileName(tileSet->getTextureSource().c_str());
    GuiLabel(Rectangle { 16, 176, 144, 24 }, sourceFileName.c_str());

    if (GuiButton(Rectangle { 16, 208, 144, 24 }, "CHANGE TEXTURE")) {
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            tileSet->setTextureSource(fsResult.path);
        }
    }

    EditorInterfaceService& ui = Editor::getUi();
    ui.drawTooltip(Rectangle { 16, 176, 144, 24 }, tileSet->getTextureSource());
}

