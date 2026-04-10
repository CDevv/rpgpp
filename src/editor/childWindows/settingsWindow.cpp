#include "childWindows/settingsWindow.hpp"

#include "TGUI/Widgets/TabContainer.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/settingsPanel/general.hpp"
#include "childWindows/settingsPanel/hotkeys.hpp"

SettingsWindow::SettingsWindow() : PopupWindow("SettingsWindow") {
	this->currentWindow->setSize("540", "360");
	this->currentWindow->setResizable(true);
	bindTranslation<tgui::ChildWindow>(this->currentWindow, "menu.options._label", &tgui::ChildWindow::setTitle);

	tgui::TabContainer::Ptr tabContainer = tgui::TabContainer::create();
	tabContainer->setPosition(0, 0);
	tabContainer->setSize({"100%", "100%"});

	general = std::make_shared<SettingsPanelGeneral>(tabContainer);
	hotkeys = std::make_shared<SettingsPanelHotkeys>(tabContainer);
	this->currentWindow->add(tabContainer);
}
