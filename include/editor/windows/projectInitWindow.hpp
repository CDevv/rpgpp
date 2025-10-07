#ifndef _RPGPP_EDITOR_PROJECTINITWINDOW_H
#define _RPGPP_EDITOR_PROJECTINITWINDOW_H

#include <raylib.h>
#include <string>

class ProjectInitWindow {
private:
	Rectangle rect;
	bool active;
	bool titleEditMode;
    std::string titleText;
    char title[256];
    bool hasSetDirPath;
    std::string dirPath;
public:
	ProjectInitWindow();
	ProjectInitWindow(Rectangle rect);
	void setActive();
	void closeWindow();
	void draw();
};

#endif
