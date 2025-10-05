#include "tileSetPropertiesBox.hpp"
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

    if (ImGui::Button("Change Texture..", ImVec2(-1, 0))) {
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            tileSet->setTextureSource(fsResult.path);
        }
    }
}
