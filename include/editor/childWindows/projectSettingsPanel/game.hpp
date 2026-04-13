#ifndef _RPGPP_PROJECTSETTINGSPANEL_GAME_H
#define _RPGPP_PROJECTSETTINGSPANEL_GAME_H

#include "childWindows/settingsPanel/base.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"

class ProjectSettingsPanelGame : public SettingsPanelBase {
public:
	FileField::Ptr defaultRoom;
	FileField::Ptr playerActor;
	IntField::Ptr tileSize;

	ProjectSettingsPanelGame(tgui::TabContainer::Ptr tabContainer);
	void setup(Project *project);
};

#endif