#ifndef _RPGPP_PROJECTSETTINGSWINDOW_H
#define _RPGPP_PROJECTSETTINGSWINDOW_H

#include "TGUI/Widgets/EditBox.hpp"
#include "childWindows/popupWindow.hpp"
#include "widgets/propertyFields/textField.hpp"

class ProjectSettingsWindow : public PopupWindow {
private:
	TextField::Ptr titleField;

public:
	ProjectSettingsWindow();
	void open() override;
};

#endif