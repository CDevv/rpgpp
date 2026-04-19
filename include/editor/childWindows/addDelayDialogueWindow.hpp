#ifndef RPGPP_ADDDELAYDIALOGUEWINDOW_HPP
#define RPGPP_ADDDELAYDIALOGUEWINDOW_HPP


#include "TGUI/Widgets/SpinControl.hpp"
#include "childWindows/popupWindow.hpp"
#include "widgets/dialogueEditor.hpp"
class AddDelayDialogueWindow : public PopupWindow {
	private:
		std::shared_ptr<DialogueEditor> editor;

		tgui::SpinControl::Ptr delaySpin;

	public:
		AddDelayDialogueWindow();

		void open(std::shared_ptr<DialogueEditor> editor);
};

#endif // RPGPP_ADDDELAYDIALOGUEWINDOW_HPP
