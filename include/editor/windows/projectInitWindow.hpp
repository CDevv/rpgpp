#ifndef _RPGPP_EDITOR_PROJECTINITWINDOW_H
#define _RPGPP_EDITOR_PROJECTINITWINDOW_H

#include <raylib.h>
#include <string>

#include "windowPopup.hpp"

class ProjectInitWindow : public WindowPopup {
private:
	Rectangle rect;
	bool titleEditMode;
    std::string titleText;
    char title[256];
    bool hasSetDirPath;
    std::string dirPath;
public:
	ProjectInitWindow();
	ProjectInitWindow(Rectangle rect);
	void openWindow();
	void closeWindow();
	void draw();
};

#endif
