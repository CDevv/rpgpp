#include "actorPropertiesBox.hpp"
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <cstdio>

ActorPropertiesBox::ActorPropertiesBox() {}

ActorPropertiesBox::ActorPropertiesBox(Rectangle rect)
{
    actor = nullptr;

    rect.height += 2;
	this->rect = rect;

	scrollVec = Vector2 { 0, 0 };
    viewRec = Rectangle { 0 };
}

void ActorPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();
    
    if (fs.getType() == FILE_ACTOR) {
        this->actor = fs.getActor();
    }
}

void ActorPropertiesBox::update()
{

}

void ActorPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

	Rectangle contentRec = rect;
    contentRec.height += 100;
    contentRec.width -= 16;

    GuiScrollPanel(rect, "Actor Props", contentRec, &scrollVec, &viewRec);

    BeginScissorMode(viewRec.x, viewRec.y, viewRec.width, viewRec.height);

    GuiToggle(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 8, (viewRec.width - 16), 24 }, "Collision View", &collisionViewActive);

    //tileset source
    GuiLine(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 32, (viewRec.width - 16), 16 }, NULL);
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 48, (viewRec.width - 16), 24 }, "TileSet");

    std::string tileSetSource = this->actor->getTileSetSource();
    GuiLabel(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 72, (viewRec.width - (16 + 24)), 24 }, tileSetSource.c_str());

    if (GuiButton(Rectangle { viewRec.x + 8 + (viewRec.width - (16 + 24)), viewRec.y + scrollVec.y + 72, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
        FS_Result fsResult = fs.openTileSetResource();
        actor->setTileSet(fsResult.path);
    }

    EndScissorMode();

    EditorInterfaceService& ui = Editor::getUi();
    ui.drawTooltip(Rectangle { viewRec.x + 8, viewRec.y + scrollVec.y + 72, (viewRec.width - (16 + 24)), 24 }, this->actor->getTileSetSource());
}