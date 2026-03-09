#ifndef RPGPP_POPUPWINDOW_H
#define RPGPP_POPUPWINDOW_H

#include "TGUI/Widgets/ChildWindow.hpp"
#include <string>
class PopupWindow {
  public:
	bool windowIsOpen = false;

	tgui::ChildWindow::Ptr currentWindow;
	PopupWindow(const std::string &title);
	~PopupWindow() { close(); };

	void open();
	void close();
};

#endif /* RPGPP_POPUPWINDOW_H */
