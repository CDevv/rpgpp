#include "projectMenuBox.hpp"

ProjectMenuBox::ProjectMenuBox() {}

ProjectMenuBox::ProjectMenuBox(Rectangle rect)
{
	this->rect = rect;
}

void ProjectMenuBox::draw()
{
	DrawRectangleLinesEx(rect, 1.0f, GRAY);
}