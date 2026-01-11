#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include "mainView.hpp"
#include "panelView.hpp"
#include "projectMenuBox.hpp"
#include "resourceViewerBox.hpp"
#include "tabList.hpp"
#include "windowContainer.hpp"
#include "worldViewBox.hpp"
#include <imgui.h>
#include <memory>
#include <raylib.h>
#include <rlImGui.h>
#include <string>

#include "edui.hpp"

class MouseInputComponent;

class EditorInterfaceService {
private:
  EdUi::Appearance appearance;
  EdUi::UiState state;
  Font uiFont;
  MainView mainView;
  ProjectMenuBox projectMenu;
  TabList tabList;
  WindowContainer windowContainer;
  std::unique_ptr<WorldViewBox> worldView;
  std::unique_ptr<PanelView> panelView;
  ResourceViewerBox resourceView;
  Vector2 mousePos;
  Vector2 hoverPos;
  bool mouseLock;
  ViewBoxLayer mouseBoxLayer;
  bool demoGuiActive;
  void drawMainView();
  void drawProjectView();
  char testStr[256];

public:
  EditorInterfaceService();
  ~EditorInterfaceService();
  void setInitial();
  void unload() const;
  void update();
  void draw();
  EdUi::Appearance &getAppearance();
  EdUi::UiState &getState();
  Font getFont() const;
  void setMouseLock(bool value);
  bool getMouseLock() const;
  void setMouseBoxLayer(ViewBoxLayer boxLayer);
  ViewBoxLayer getMouseBoxLayer() const;
  TabList &getTabList();
  WindowContainer &getWindowContainer();
};

#endif
