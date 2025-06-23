#include "tileSetPropertiesBox.hpp"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

TileSetPropertiesBox::TileSetPropertiesBox() {}

TileSetPropertiesBox::TileSetPropertiesBox(Rectangle rect)
{
    chosenTileSize = 0;
    chosenTileSizeEditMode = false;
    this->rect = rect;

    scrollVec = Vector2 { 0, 0 };
    viewRec = Rectangle { 0 };
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

    Rectangle contentRec = rect;
    contentRec.height += 100;
    contentRec.width -= 16;
    GuiScrollPanel(rect, "TileSet Props", contentRec, &scrollVec, &viewRec);

    BeginScissorMode(viewRec.x, viewRec.y, viewRec.width, viewRec.height);

    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 8, (viewRec.width - 16), 24 }, "Tile Size");
    if (GuiValueBox(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 32, (viewRec.width - 16), 24 }, NULL, &chosenTileSize, 16, 32, chosenTileSizeEditMode)) {
        chosenTileSizeEditMode = !chosenTileSizeEditMode;
    }

    GuiLine(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 56, (viewRec.width - 16), 16 }, NULL);

    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 72, (viewRec.width - 16), 24 }, "Texture");
    std::string sourceFileName = GetFileName(tileSet->getTextureSource().c_str());
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 96, (viewRec.width - (16 + 24)), 24 }, sourceFileName.c_str());

    if (GuiButton(Rectangle { viewRec.x + 112, viewRec.y + scrollVec.y + 96, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            tileSet->setTextureSource(fsResult.path);
        }
    }

    EndScissorMode();

    EditorInterfaceService& ui = Editor::getUi();
    ui.drawTooltip(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 96, (viewRec.width - (16 + 24)), 24 }, tileSet->getTextureSource());
}

