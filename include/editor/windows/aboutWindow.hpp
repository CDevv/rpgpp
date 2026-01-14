#ifndef _RPGPP_ABOUTWINDOW_H
#define _RPGPP_ABOUTWINDOW_H

#include "windowPopup.hpp"
#include <raylib.h>


class AboutWindow : public WindowPopup {
private:
  Rectangle rect{};

public:
  AboutWindow();
  AboutWindow(Rectangle rect);
  void openWindow();
  void closeWindow();
  void draw();
};

#endif
