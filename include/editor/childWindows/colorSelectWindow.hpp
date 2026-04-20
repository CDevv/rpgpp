#ifndef _RPGPP_COLORSELECTWINDOW_HPP
#define _RPGPP_COLORSELECTWINDOW_HPP

#include <memory>

#include "childWindows/popupWindow.hpp"
#include "widgets/dialogueEditor.hpp"
class ColorSelectWindow : public PopupWindow {
private:
	std::shared_ptr<DialogueEditor> editor;

public:
	ColorSelectWindow();

	void open(std::shared_ptr<DialogueEditor> editor);
};

#endif