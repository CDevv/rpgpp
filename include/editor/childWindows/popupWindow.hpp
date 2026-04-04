#ifndef RPGPP_POPUPWINDOW_H
#define RPGPP_POPUPWINDOW_H

#include "TGUI/Widgets/ChildWindow.hpp"
#include <string>
class PopupWindow {
  public:
	bool windowIsOpen = false;

	tgui::ChildWindow::Ptr currentWindow;
	explicit PopupWindow(const std::string &title);
	virtual ~PopupWindow() { PopupWindow::close(); };

	virtual void open();
	virtual void close();
};

#endif /* RPGPP_POPUPWINDOW_H */
