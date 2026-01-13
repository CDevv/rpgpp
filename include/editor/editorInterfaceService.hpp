#ifndef _RPGPP_EDITOR_INTERFACE_H
#define _RPGPP_EDITOR_INTERFACE_H

#include "mainView.hpp"
#include "panelView.hpp"
#include "projectMenuBox.hpp"
#include "resourceViewerBox.hpp"
#include "tabList.hpp"
#include "windowContainer.hpp"
#include "worldViewBox.hpp"
#include <memory>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <TGUI/Backend/raylib.hpp>

class MouseInputComponent;

class EditorInterfaceService {
private:
  std::unique_ptr<tgui::Gui> gui;
  Font uiFont{};
  MainView mainView;
  ProjectMenuBox projectMenu;
  TabList tabList;
  std::unique_ptr<WindowContainer> windowContainer;
  std::unique_ptr<WorldViewBox> worldView;
  std::unique_ptr<PanelView> panelView;
  ResourceViewerBox resourceView;
  Vector2 mousePos{};
  Vector2 hoverPos{};
  bool mouseLock;
  ViewBoxLayer mouseBoxLayer;
  bool demoGuiActive;
  void drawMainView();
  void drawProjectView();

public:
  EditorInterfaceService();
  ~EditorInterfaceService();
  tgui::Gui* getGui();
  void setInitial();
  void unload();
  void update();
  void draw();
  Font getFont() const;
  void setMouseLock(bool value);
  bool getMouseLock() const;
  void setMouseBoxLayer(ViewBoxLayer boxLayer);
  ViewBoxLayer getMouseBoxLayer() const;
  TabList &getTabList();
  WindowContainer &getWindowContainer();
};

#endif
