#include "mapPanelView.hpp"
#include <memory>
#include <raylib.h>
#include <raygui.h>
#include "collisionInfoPanel.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "interactableInfoPanel.hpp"
#include "propertiesBox.hpp"
#include "worldViewBox.hpp"

MapPanelView::MapPanelView()
{
}

MapPanelView::MapPanelView(Rectangle rect)
{
    this->rect = rect;

    actionModeToggle = 0;
    layoutModeToggle = 0;
    layoutDropdownEditMode = false;

    Rectangle toolsRect = Rectangle
    {
        rect.x, rect.y, static_cast<float>(rect.width - 284), 30
    };
    toolsBox = MapToolsBox(toolsRect);

    Rectangle windowRect = Rectangle
    {
        rect.x, rect.y + 34,
        static_cast<float>(rect.width - 284), static_cast<float>(rect.height - 34)
    };
    worldView = std::make_unique<WorldViewBox>(windowRect, FILE_ROOM);

    Rectangle tileSetWindowRect = Rectangle
    {
        (rect.x + windowRect.width + 4), (rect.y + 28),
        (280), (8)
    };
    tileSetWindowRect.height = tileSetWindowRect.width - 36;
    tileSetView = std::make_unique<WorldViewBox>(tileSetWindowRect, FILE_TILESET);

    tileSetView->enableTileSelection();
    worldView->setActionMode(ACTION_PLACE);

    collisionInfo = CollisionInfoPanel(tileSetWindowRect);
    interactableInfo = InteractableInfoPanel(tileSetWindowRect);

    Rectangle propRect = Rectangle
    {
        (tileSetWindowRect.x), (tileSetWindowRect.y + tileSetWindowRect.height + 4),
        280, (rect.height + rect.y - (tileSetWindowRect.y + tileSetWindowRect.height + 4) + 2)
    };
    propRect.width = tileSetWindowRect.width;
    propBox = PropertiesBox(propRect);
}

void MapPanelView::setInitial()
{
    propBox.setDefaults();
}

void MapPanelView::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    toolsBox.update();

    worldView->setMouseLock(ui.getMouseLock());

    worldView->setRoom(fs.getRoom());
    worldView->update();

    if (fs.getType() == FILE_ROOM && fs.getRoom() != nullptr) {
        tileSetView->setTileSet(fs.getRoom()->getTileMap()->getTileSet());
    }
    tileSetView->update();

    worldView->setSelectedTile(tileSetView->getSelectedTile());

    if (fs.fileIsOpen() && fs.getType() == FILE_ROOM) {
        propBox.update();
    }

    RoomAction actionMode = toolsBox.getActionMode();
    worldView->setActionMode(actionMode);
    collisionInfo.setActionMode(actionMode);

    interactableInfo.update();
    interactableInfo.setActionMode(actionMode);

    RoomLayer layerMode = static_cast<RoomLayer>(layoutModeToggle);
    worldView->setLayerMode(layerMode);

    worldView->setCurrentInteractableType(interactableInfo.getType());

    if (fs.getType() == FILE_ROOM && fs.getRoom() != nullptr) {
        Vector2 selectedTileWorldPos = worldView->getSelectedWorldTile();
        Interactable selectedInteractable = fs.getRoom()->getInteractables().get(selectedTileWorldPos.x, selectedTileWorldPos.y);
        interactableInfo.setSelectedInteractable(selectedInteractable);
    }
}

void MapPanelView::draw()
{
    if (layoutDropdownEditMode) GuiLock();

    toolsBox.draw();
    worldView->draw();

    RoomLayer layerMode = static_cast<RoomLayer>(layoutModeToggle);
    switch (layerMode) {
        case LAYER_TILES:
            tileSetView->draw();
            break;
        case LAYER_COLLISIONS:
            collisionInfo.draw();
            break;
        case LAYER_INTERACTABLES:
            interactableInfo.draw();
            break;
    }

    propBox.draw();

    Rectangle windowRect = worldView->getWindowRect();
    Rectangle optionsRect = Rectangle 
    {
        (rect.x + windowRect.width + 4), (rect.y),
        280, 24
    };
    Rectangle singleOptionRect = optionsRect;
    singleOptionRect.width /= 3;
    singleOptionRect.width -= 2;

    Rectangle layoutDropdown = optionsRect;
    if (GuiDropdownBox(layoutDropdown, "Tiles;Collisions;Interactables", &layoutModeToggle, layoutDropdownEditMode)) {
        layoutDropdownEditMode = !layoutDropdownEditMode;
    }
}
