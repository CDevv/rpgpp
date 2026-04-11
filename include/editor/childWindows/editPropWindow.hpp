#ifndef _RPGPP_EDITPROPWINDOW_H
#define _RPGPP_EDITPROPWINDOW_H

#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "childWindows/popupWindow.hpp"
#include "interactable.hpp"
#include "nlohmann/json_fwd.hpp"
#include "widgets/propertiesBox.hpp"

class EditPropWindow : public PopupWindow {
public:
	nlohmann::json *props;
	PropertiesBox *box;
	Interactable *interactable;
	std::string propName;

	tgui::Label::Ptr nameLabel;
	tgui::ComboBox::Ptr dropdown;

	EditPropWindow();
};

#endif