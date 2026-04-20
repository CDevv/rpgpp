#ifndef _RPGPP_NEWPROPWINDOW_H
#define _RPGPP_NEWPROPWINDOW_H

#include "childWindows/popupWindow.hpp"
#include "interactable.hpp"
#include "nlohmann/json_fwd.hpp"
#include "widgets/propertiesBox.hpp"

class NewPropWindow : public PopupWindow {
public:
	nlohmann::json *props;
	PropertiesBox *box;
	Interactable *interactable;
	NewPropWindow();
};

#endif