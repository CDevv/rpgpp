#ifndef RPGPP_SETTINGSPANEL_BASE_H
#define RPGPP_SETTINGSPANEL_BASE_H

#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/TabContainer.hpp"
#include "bindTranslation.hpp"
#include "services/translationService.hpp"

class SettingsPanelBase {
protected:
	tgui::Panel::Ptr panel;

public:
	SettingsPanelBase(tgui::TabContainer::Ptr tabContainer, std::string name) {
		panel = tabContainer->addTab(name);
		bindTranslationWithCallback<tgui::TabContainer>(
			tabContainer, [this, name](tgui::TabContainer::Ptr tabContainer, TranslationService &ts) {
				int idx = tabContainer->getIndex(panel);
				tabContainer->changeTabText(idx, ts.getKey(name));
			});
	}
};

#endif
