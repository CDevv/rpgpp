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
    chosenTileWidth = 0;
    chosenTileHeight = 0;

    chosenTileSizeEditMode = false;
    chosenTileWidthEditMode = false;
    chosenTileHeightEditMode = false;

    multiSizeCheckBox = true;

    this->rect = rect;

    scrollVec = Vector2 { 0, 0 };
    viewRec = Rectangle { 0 };

    tileSizeArr = { 0, 0 };
}

void TileSetPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileSet *tileSet = fs.getTileSet();
    this->tileSet = tileSet;
    chosenTileSize = tileSet->getTileSize().x;
    chosenTileWidth = tileSet->getTileSize().x;
    chosenTileHeight = tileSet->getTileSize().y;

    tileSizeArr[0] = tileSet->getTileSize().x;
    tileSizeArr[1] = tileSet->getTileSize().y;

    if (chosenTileWidth != chosenTileHeight) {
        multiSizeCheckBox = false;
    }
}

void TileSetPropertiesBox::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    TileSet *tileSet = fs.getTileSet();
    //this->tileSet = tileSet;

    if (multiSizeCheckBox) {
        if (chosenTileSize >= 16) {
            tileSet->setTileSize(chosenTileSize);
        }
    } else {

        /*if (chosenTileWidth >= 16 && chosenTileHeight >= 16) {
            Vector2 sizeVec = Vector2 { static_cast<float>(chosenTileWidth), static_cast<float>(chosenTileHeight) };
            tileSet->setTileSizeVector(sizeVec);
        }*/

        if (tileSizeArr[0] >= 16 && tileSizeArr[1] >= 16) {
            Vector2 sizeVec = Vector2 { static_cast<float>(tileSizeArr[0]), static_cast<float>(tileSizeArr[1]) };
            tileSet->setTileSizeVector(sizeVec);
        }
    }
}

void TileSetPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    TileSet *tileSet = fs.getTileSet();
    //this->tileSet = tileSet;

    ImGui::Checkbox("Square Tiles?", &multiSizeCheckBox);

    if (multiSizeCheckBox) {
        ImGui::InputInt("Tile Size", &chosenTileSize);
    } else {
        ImGui::InputInt2("Tile Size", tileSizeArr.data());
    }

    std::string sourceFileName = GetFileName(tileSet->getTextureSource().c_str());
    sourceFileName.push_back('\0');

    ImGui::InputText("Texture", const_cast<char*>(sourceFileName.data()), sourceFileName.size(), ImGuiInputTextFlags_ReadOnly);
    //ImGui::SetItemTooltip("Tooltip.");
    

    if (ImGui::Button("Change Texture..", ImVec2(-1, 0))) {
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            tileSet->setTextureSource(fsResult.path);
        }
    }

    /*
    Rectangle contentRec = rect;
    contentRec.height += 100;
    contentRec.width -= 16;
    GuiScrollPanel(rect, "TileSet Props", contentRec, &scrollVec, &viewRec);

    BeginScissorMode(viewRec.x, viewRec.y, viewRec.width, viewRec.height);

    GuiCheckBox(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 32, 24, 24 }, "Square Tiles", &multiSizeCheckBox);

    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 8, (viewRec.width - 16), 24 }, "Tile Size");

    if (multiSizeCheckBox) {
        if (GuiValueBox(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 64, (viewRec.width - 16), 24 }, NULL, &chosenTileSize, 16, 100, chosenTileSizeEditMode)) {
            chosenTileSizeEditMode = !chosenTileSizeEditMode;
        }
    } else {
        float halfWidth = (viewRec.width - 16) / 2;
        if (GuiValueBox(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 64, halfWidth, 24 }, NULL, &chosenTileWidth, 16, 100, chosenTileWidthEditMode)) {
            chosenTileWidthEditMode = !chosenTileWidthEditMode;
        }
        if (GuiValueBox(Rectangle { viewRec.x + 8 + halfWidth, viewRec.y + scrollVec.y + 64, halfWidth, 24 }, NULL, &chosenTileHeight, 16, 100, chosenTileHeightEditMode)) {
            chosenTileHeightEditMode = !chosenTileHeightEditMode;
        }
    }

    GuiLine(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 88, (viewRec.width - 16), 16 }, NULL);

    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 104, (viewRec.width - 16), 24 }, "Texture");
    std::string sourceFileName = GetFileName(tileSet->getTextureSource().c_str());
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 128, (viewRec.width - (16 + 24)), 24 }, sourceFileName.c_str());

    if (GuiButton(Rectangle { viewRec.x + 8 + (viewRec.width - (16 + 24)), viewRec.y + scrollVec.y + 128, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            tileSet->setTextureSource(fsResult.path);
        }
    }

    EndScissorMode();

    EditorInterfaceService& ui = Editor::getUi();
    ui.drawTooltip(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 128, (viewRec.width - (16 + 24)), 24 }, tileSet->getTextureSource());
    */
}

