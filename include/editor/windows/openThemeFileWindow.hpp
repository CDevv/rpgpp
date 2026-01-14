

#include "raylib.h"
#include "windowPopup.hpp"

class OpenThemeFileWindow : public WindowPopup {
private:
  Rectangle rect{};

public:
  OpenThemeFileWindow();
  OpenThemeFileWindow(Rectangle rect);
  void openWindow();
  void closeWindow();
  void draw();
};