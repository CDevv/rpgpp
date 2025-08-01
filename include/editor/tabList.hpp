#ifndef _RPGPP_EDITOR_TABLIST_H
#define _RPGPP_EDITOR_TABLIST_H

#include <string>
#include <vector>
#include <raylib.h>

struct TabData
{
	std::string title;
};

class TabList {
private:
	Rectangle rect;
	std::vector<TabData> tabs;
	int activeIndex;
	float tabWidth;
	bool drawOverflow;
	Rectangle scissorRect;
	Vector2 scissorOffset;
	float maxScissorOffset;
public:
	TabList();
	TabList(Rectangle rect);
	void addItem(std::string title);
	void update();
	void draw();
};

#endif