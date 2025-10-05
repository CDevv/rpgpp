#ifndef _RPGPP_EDITOR_PROJECTMENUBOX_H
#define _RPGPP_EDITOR_PROJECTMENUBOX_H

#include <raylib.h>

class ProjectMenuBox {
private:
	Rectangle rect;
public:
	ProjectMenuBox();
	ProjectMenuBox(Rectangle rect);
	void setRect(Rectangle rect);
	void draw();
};

#endif