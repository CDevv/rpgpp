#include "tabList.hpp"
#include "tabButton.hpp"
#include <raymath.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

TabList::TabList() {}

TabList::TabList(Rectangle rect)
{
	this->rect = rect;
	this->scissorRect = rect;
	this->scissorOffset = Vector2 { 0, 0 };
	this->activeIndex = 0;
	this->tabWidth = 120.0f;
	this->drawOverflow = false;
}

void TabList::addItem(std::string title)
{
	bool exists = false;
	for (std::vector<TabData>::iterator i = tabs.begin(); i != tabs.end(); ++i)
	{
		if (i->title == title) {
			exists = true;
			break;
		}
	}

	if (exists) return;

	this->tabs.push_back(TabData {title});
	activeIndex = tabs.size() - 1;
}

void TabList::update()
{
	if ((tabs.size() * tabWidth) > rect.width) {
		drawOverflow = true;
		scissorRect = rect;
		scissorRect.width -= (rect.height * 2);
		maxScissorOffset = (tabs.size() * tabWidth) - scissorRect.width;
	} else {
		drawOverflow = false;
		scissorRect = rect;
		scissorOffset = Vector2 { 0, 0 };
	}
}

void TabList::draw()
{
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	int index = 0;
	bool actionLock = false;

	BeginScissorMode(scissorRect.x, scissorRect.y, scissorRect.width, scissorRect.height);
	for (std::vector<TabData>::iterator i = tabs.begin(); i != tabs.end();)
	{
		Rectangle tabRect = Rectangle
		{
			rect.x + (index * tabWidth) + scissorOffset.x, rect.y,
			tabWidth, 24
		};

		Rectangle detectRect = tabRect;
		float scissorTopRight = (scissorRect.x + scissorRect.width);
		if ((tabRect.x + tabRect.width) > scissorTopRight) {
			float diff = (tabRect.x + tabRect.width) - scissorTopRight;
			detectRect.width -= diff;
		}

		TabButtonState tabState;
		if (activeIndex == index) {
			tabState = EditorGuiTabButton(tabRect, i->title, true, scissorRect);
		} else {
			tabState = EditorGuiTabButton(tabRect, i->title, false, scissorRect);
		}

		//DrawRectangleLinesEx(detectRect, 2.0f, MAROON);

		if (tabState == TABSTATE_PRESSED) {
			activeIndex = index;
			fs.setActiveProjectFile(activeIndex);
			ui.setInitial();
		} else if (tabState == TABSTATE_X_PRESSED) {
			if (!actionLock) {
				tabs.erase(i);
				actionLock = true;
				scissorOffset = Vector2 { 0, 0 };
				fs.closeProjectFile(index);
				printf("%i\n", index);

				if (activeIndex == index) {
					activeIndex = 0;
					fs.setActiveProjectFile(0);
					ui.setInitial();
				}
			}
		}

		index++;

		if (tabState != TABSTATE_X_PRESSED) {
			++i;
		}
	}
	EndScissorMode();

	if (drawOverflow) {
		Rectangle leftButtonRect = Rectangle
		{
			rect.x + rect.width - (rect.height * 2), rect.y,
			rect.height, rect.height
		};
		Rectangle rightButtonRect = Rectangle
		{
			rect.x + rect.width - (rect.height), rect.y,
			rect.height, rect.height
		};

		if (GuiButton(leftButtonRect, GuiIconText(ICON_ARROW_LEFT_FILL, NULL))) {
			//scissorOffset.x += 20;
			scissorOffset.x = Clamp(scissorOffset.x + 20, -maxScissorOffset, 0);
		}
		if (GuiButton(rightButtonRect, GuiIconText(ICON_ARROW_RIGHT_FILL, NULL))) {
			//scissorOffset.x -= 20;
			scissorOffset.x = Clamp(scissorOffset.x - 20, -maxScissorOffset, 0);
		}
	}

	DrawRectangleLinesEx(rect, 1.0f, GRAY);
}