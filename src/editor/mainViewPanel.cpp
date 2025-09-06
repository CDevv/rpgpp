#include <mainViewPanel.hpp>
#include <raygui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"

MainViewPanel::MainViewPanel() {}

MainViewPanel::MainViewPanel(Rectangle rect)
{
	this->rect = rect;
}

void MainViewPanel::update()
{

}

void MainViewPanel::draw()
{
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	GuiPanel(rect, "RPG++");

	if (GuiButton(Rectangle { rect.x + 8, rect.y + 32, (rect.width - 16), 24 }, GuiIconText(ICON_FOLDER_OPEN, "Open Project.."))) {
		fs.promptOpenProject();
		ui.setInitial();
	}

	if (GuiButton(Rectangle { rect.x + 8, rect.y + 64, (rect.width - 16), 24 }, GuiIconText(ICON_FOLDER_ADD, "Create Project.."))) {
		//fs.promptOpenProject();
		//ui.setInitial();
		ui.getWindowContainer().openProjectInit();
	}
}