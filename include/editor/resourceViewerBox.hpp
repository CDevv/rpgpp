#ifndef _RPGPP_EDITOR_RESOURCEVIEWERBOX_H
#define _RPGPP_EDITOR_RESOURCEVIEWERBOX_H

#include <raylib.h>
#include <string>

class ResourceViewerBox {
private:
  Rectangle rect;
  int dropdownActive;
  bool deleteConfirmOpen;
  std::string deleteConfirmPath;
  void onNewButton() const;
  void drawResourcesList();
  void drawContextMenu(const std::string &resPath);

public:
  ResourceViewerBox();
  explicit ResourceViewerBox(Rectangle rect);
  void setRect(Rectangle m_rect);
  void update() const;
  void draw();
};

#endif
