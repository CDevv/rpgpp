#ifndef _RPGPP_EDITOR_TABBUTTON_H
#define _RPGPP_EDITOR_TABBUTTON_H

#include <raylib.h>
#include <raygui.h>
#include <string>

enum TabButtonState {
	TABSTATE_NORMAL,
	TABSTATE_FOCUSED,
	TABSTATE_PRESSED,
	TABSTATE_X_HOVER,
	TABSTATE_X_PRESSED
};

TabButtonState EditorGuiTabButton(Rectangle rect, std::string text, bool isActive);

#endif