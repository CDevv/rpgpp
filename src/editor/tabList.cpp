#include "tabList.hpp"
#include "tabButton.hpp"

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
	this->tabs.push_back(TabData {title});
}

void TabList::update()
{
	if ((tabs.size() * tabWidth) > rect.width) {
		drawOverflow = true;
		scissorRect = rect;
		scissorRect.width -= (rect.height * 2);
	}
}

void TabList::draw()
{
	int index = 0;

	BeginScissorMode(scissorRect.x, scissorRect.y, scissorRect.width, scissorRect.height);
	for (std::vector<TabData>::iterator i = tabs.begin(); i != tabs.end(); ++i)
	{
		Rectangle tabRect = Rectangle
		{
			rect.x + (index * tabWidth) + scissorOffset.x, rect.y,
			tabWidth, 24
		};

		TabButtonState tabState;
		if (activeIndex == index) {
			tabState = EditorGuiTabButton(tabRect, i->title, true);
		} else {
			tabState = EditorGuiTabButton(tabRect, i->title, false);
		}

		if (tabState == TABSTATE_PRESSED) {
			activeIndex = index;
		}

		index++;
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
			scissorOffset.x += 20;
		}
		if (GuiButton(rightButtonRect, GuiIconText(ICON_ARROW_RIGHT_FILL, NULL))) {
			scissorOffset.x -= 20;
		}
	}

	DrawRectangleLinesEx(rect, 1.0f, GRAY);
}