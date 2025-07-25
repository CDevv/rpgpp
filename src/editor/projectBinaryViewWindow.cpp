#include "projectBinaryViewWindow.hpp"
#include <raygui.h>

ProjectBinaryViewWindow::ProjectBinaryViewWindow() {}

ProjectBinaryViewWindow::ProjectBinaryViewWindow(Rectangle rect)
{
	this->rect = rect;
	this->active = false;
	this->dataAvailable = false;
	this->data = std::unique_ptr<GameData>{};
}

void ProjectBinaryViewWindow::setActive()
{
	this->active = true;
}

void ProjectBinaryViewWindow::closeWindow()
{
	this->data.reset();
	this->active = false;
}

void ProjectBinaryViewWindow::draw()
{
	if (active) {
		if (dataAvailable) {
			if (GuiWindowBox(rect, data->title.c_str())) {
				closeWindow();
			}

			for (auto tileSetData : data->tilesets) {
				unsigned char* imageData = tileSetData.image.data();
				Image image = LoadImageFromMemory(".png", imageData, tileSetData.dataSize);
				Texture texture = LoadTextureFromImage(image);

				DrawTexture(texture, rect.x, rect.y, WHITE);
			}
		} else {
			if (GuiWindowBox(rect, "GameData")) {
				closeWindow();
			}
		}
	}
}

void ProjectBinaryViewWindow::setData(GameData data)
{
	this->data = std::make_unique<GameData>(data);
	this->dataAvailable = true;
}