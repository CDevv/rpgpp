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
    worldView = std::make_unique<WorldViewBox>(windowRect, FILE_ROOM, VIEWBOX_LAYER_BASE);

    Rectangle tileSetWindowRect = Rectangle
    {
        (rect.x + windowRect.width + 4), (rect.y + 28),
        (280), (8)
    };
    tileSetWindowRect.height = tileSetWindowRect.width - 36;
    tileSetView = std::make_unique<WorldViewBox>(tileSetWindowRect, FILE_TILESET, VIEWBOX_LAYER_BASE);
    tileSetView->setWindowTitle("TileSet");

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

void MapPanelView::setRect(Rectangle rect)
{
    this->rect = rect;

    Rectangle toolsRect = Rectangle
    {
        rect.x, rect.y, static_cast<float>(rect.width * 0.7), 30
    };
    toolsBox.setRect(toolsRect);

    Rectangle windowRect = Rectangle
    {
        rect.x, rect.y + 36,
        static_cast<float>(rect.width * 0.7), static_cast<float>(rect.height - 36)
    };
    worldView->setRect(windowRect);

    float remainingHeight = rect.height - (28 + 4);
    Rectangle tileSetWindowRect = Rectangle
    {
        (rect.x + windowRect.width + 4), (rect.y + 28 + 8),
        (rect.width * 0.3f) - 8, (remainingHeight / 2)
    };
    tileSetWindowRect.height = tileSetWindowRect.width - 36;
    tileSetView->setRect(tileSetWindowRect);

    remainingHeight -= tileSetWindowRect.height;

    Rectangle propRect = Rectangle
    {
        (tileSetWindowRect.x), (tileSetWindowRect.y + tileSetWindowRect.height + 4),
        tileSetWindowRect.width, remainingHeight - 8
    };
    propBox.setRect(propRect);
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
        if (selectedTileWorldPos.x != -1) {
            Interactable* selectedInteractable = fs.getRoom()->getInteractables().get(selectedTileWorldPos.x, selectedTileWorldPos.y);
            interactableInfo.setSelectedInteractable(selectedInteractable);
        }
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
        default:
            break;
    }

    propBox.draw();

    //Choosing on what layer to work on..
    ImGui::SetNextWindowPos(ImVec2 { rect.x + (rect.width * 0.7f) + 4, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { (rect.width * 0.3f) - 8, 28 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    if (ImGui::Begin("MapLayerSelect", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

        ImGui::Combo("Layer", &layoutModeToggle, "Tiles\0Collisions\0Interactables\0");

        ImGui::End();
    }
    ImGui::PopStyleVar();
}
