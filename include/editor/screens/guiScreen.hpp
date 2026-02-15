#ifndef _RPGPP_BASE_GUI_WIDGET_H
#define _RPGPP_BASE_GUI_WIDGET_H

#include <TGUI/Widgets/Group.hpp>
#include <string>

class UIScreen {
  public:
	virtual ~UIScreen() = default;
	UIScreen() = default;

	void addElement();

	virtual void initItems(tgui::Group::Ptr layout) {
		// this is a base method, meaning other objects should extend it
		// to create widgets.
	}

	virtual void mouseMove(int x, int y) {

	}

	virtual void leftMouseReleased(int x, int y) {

	}

	virtual void unloadScreen() {
		// this is a base method when you need to unload textures and such.
	}

	virtual const std::string getNameOfScreen() { return "base"; }
};
#endif
