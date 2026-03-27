#ifndef RPGPP_SETTINGSWINDOW_H
#define RPGPP_SETTINGSWINDOW_H

#include "childWindows/popupWindow.hpp"
#include "settingsPanel/general.hpp"
#include "settingsPanel/hotkeys.hpp"

class SettingsWindow : public PopupWindow {
  private:
	std::shared_ptr<SettingsPanelGeneral> general;
	std::shared_ptr<SettingsPanelHotkeys> hotkeys;

  public:
	SettingsWindow();
};

#endif /* RPGPP_SETTINGSWINDOW_H */
