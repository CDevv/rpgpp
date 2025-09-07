#include <mainViewPanel.hpp>
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"

MainViewPanel::MainViewPanel() {}

MainViewPanel::MainViewPanel(Rectangle rect)
{
	this->rect = rect;
	this->logoTexture = LoadTexture("logo.png");
}

void MainViewPanel::update()
{

}

void MainViewPanel::draw()
{
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	GuiPanel(rect, "RPG++");

	Rectangle originRect = {
		0, 0, static_cast<float>(logoTexture.width), static_cast<float>(logoTexture.height)
	};
	Rectangle destRect = Rectangle {
		rect.x + (((rect.width / 2) - (originRect.width / 2)) / 2), rect.y + 24, rect.width / 2, rect.width / 2
	};

	DrawTexturePro(logoTexture, originRect, destRect, Vector2 { 0, 0 }, 0.0f, WHITE);

	if (GuiButton(Rectangle { rect.x + 8, rect.y + 32 + destRect.height, (rect.width - 16), 24 }, GuiIconText(ICON_FOLDER_OPEN, "Open Project.."))) {
		fs.promptOpenProject();
		ui.setInitial();
	}

	if (GuiButton(Rectangle { rect.x + 8, rect.y + 64 + destRect.height, (rect.width - 16), 24 }, GuiIconText(ICON_FOLDER_ADD, "Create Project.."))) {
		ui.getWindowContainer().openProjectInit();
	}
}