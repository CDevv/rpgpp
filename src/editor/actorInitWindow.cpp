#include "actorInitWindow.hpp"
#include <raygui.h>
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include <nlohmann/json.hpp>

ActorInitWindow::ActorInitWindow() {}

ActorInitWindow::ActorInitWindow(Rectangle rect)
{
	this->rect = rect;
	this->active = false;

	this->titleEditMode = false;
	this->title = std::make_unique<char>();
	this->titleText = "";
	this->hasSetTileSet = false;
	this->tileSetSource = "";
}

void ActorInitWindow::setActive()
{
	active = true;
}

void ActorInitWindow::closeWindow()
{
	EditorInterfaceService& ui = Editor::getUi();
	ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
	ui.setMouseLock(false);

	active = false;
	*title = '\0';
	titleEditMode = false;
	titleText = "";
	hasSetTileSet = false;
	tileSetSource = "";
}

void ActorInitWindow::draw()
{
	if (active) {
		FileSystemService& fs = Editor::getFileSystem();
        EditorInterfaceService& ui = Editor::getUi();

		if (GuiWindowBox(rect, "New Actor..")) {
			closeWindow();
		}

		GuiLabel(Rectangle { rect.x + 8, rect.y + 40, rect.width - 16, 24 }, "Title..");
        if (GuiTextBox(Rectangle { rect.x + 8, rect.y + 64, rect.width - 16, 24 }, title.get(), 13, titleEditMode)) {
            titleEditMode = !titleEditMode;
        }

        GuiLabel(Rectangle { rect.x + 8, rect.y + 96, rect.width - 16, 24 }, "Texture..");
        Rectangle tileSetSourceLabelRect = Rectangle { rect.x + 8, rect.y + 120, rect.width - (16 + 24), 24 };
        if (hasSetTileSet) {
            GuiLabel(tileSetSourceLabelRect, tileSetSource.c_str());
        } else {
            GuiLabel(tileSetSourceLabelRect, "Not set..");
        }

        if (GuiButton(Rectangle { rect.x + 8 + (rect.width - (16 + 24)), rect.y + 120, 24, 24 }, GuiIconText(ICON_FILE_OPEN, NULL))) {
            FS_Result fsResult = fs.openTileSetResource();
            if (fsResult.result == NFD_OKAY) {
                tileSetSource = fsResult.path;
                hasSetTileSet = true;
            }
        }

        if (GuiButton(Rectangle { rect.x + 184, rect.y + (rect.height - (24 + 8)), 120, 24 }, "Submit..")) {
            titleText = title.get();

            if (titleText.empty()) return;
            if (!hasSetTileSet) return;

            std::unique_ptr<TileSet> tileSet = std::make_unique<TileSet>(tileSetSource);
            Actor actor(std::move(tileSet), Vector2 { 0, 0 }, tileSetSource);

            nlohmann::json actorJson = actor.dumpJson();
            std::string jsonString = actorJson.dump(4);

            std::string filePath = std::string("actors/").append(titleText).append(".ractor");
            SaveFileText(filePath.c_str(), const_cast<char*>(jsonString.data()));

            fs.openProjectFile(filePath);
            ui.setInitial();
            	
            closeWindow();
        }

		if (active) {
            if (CheckCollisionPointRec(GetMousePosition(), rect)) {
                ui.setMouseBoxLayer(VIEWBOX_LAYER_WINDOW);
                ui.setMouseLock(true);
            } else {
                ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
                ui.setMouseLock(false);
            }
        }
	}
}