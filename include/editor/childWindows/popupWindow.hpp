#ifndef RPGPP_POPUPWINDOW_H
#define RPGPP_POPUPWINDOW_H

#include <string>

#include "TGUI/Widgets/ChildWindow.hpp"
class PopupWindow {
public:
	bool windowIsOpen = false;

	tgui::ChildWindow::Ptr currentWindow;
	explicit PopupWindow(const std::string &title);
	~PopupWindow() { close(); };

	virtual void open();
	void close();
};

#endif /* RPGPP_POPUPWINDOW_H */
