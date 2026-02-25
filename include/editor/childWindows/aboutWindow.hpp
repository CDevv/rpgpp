#ifndef RPGPP_ABOUTWINDOW_H
#define RPGPP_ABOUTWINDOW_H

#include "TGUI/Widgets/Label.hpp"
#include "childWindows/popupWindow.hpp"
#include <string>
class AboutWindow : public PopupWindow {
  public:
	AboutWindow(const std::string &title);
};

#endif /* RPGPP_ABOUTWINDOW_H */
