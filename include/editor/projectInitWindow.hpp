#ifndef _RPGPP_EDITOR_PROJECTINITWINDOW_H
#define _RPGPP_EDITOR_PROJECTINITWINDOW_H

#include <raylib.h>
#include <string>
#include <memory>

class ProjectInitWindow {
private:
	Rectangle rect;
	bool active;
	bool titleEditMode;
    std::string titleText;
    std::unique_ptr<char> title;
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