#include <TGUI/Widgets/MessageBox.hpp>

#include "base.hpp"

#ifndef RPGPP_SETTINGSPANEL_GENERAL_H
#define RPGPP_SETTINGSPANEL_GENERAL_H
class SettingsPanelGeneral : public SettingsPanelBase {
	tgui::MessageBox::Ptr promptUserBox;

	public:
		explicit SettingsPanelGeneral(tgui::TabContainer::Ptr tabContainer);
};

#endif
