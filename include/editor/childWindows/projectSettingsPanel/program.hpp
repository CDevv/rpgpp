#ifndef _RPGPP_PROJECTSETTINGSPANEL_PROGRAM_H
#define _RPGPP_PROJECTSETTINGSPANEL_PROGRAM_H

#include "childWindows/settingsPanel/base.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"

class ProjectSettingsPanelProgram : public SettingsPanelBase {
public:
	TextField::Ptr titleField;
	FileField::Ptr programIcon;
	IntField::Ptr windowSizeX;
	IntField::Ptr windowSizeY;
	BoolField::Ptr resizeable;
	SelectField::Ptr windowStateFlag;
	IntField::Ptr targetFPS;

	ProjectSettingsPanelProgram(tgui::TabContainer::Ptr tabContainer);
	void setup(Project *project);
};

#endif
