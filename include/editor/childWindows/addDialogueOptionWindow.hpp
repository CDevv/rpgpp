#ifndef _RPGPP_CHILDWINDOWS_ADDDIALOGUEOPTIONWINDOW_H
#define _RPGPP_CHILDWINDOWS_ADDDIALOGUEOPTIONWINDOW_H

#include "childWindows/popupWindow.hpp"
#include "dialogue.hpp"
#include "fileViews/dialogueFileView.hpp"

class AddDialogueOptionWindow : public PopupWindow {
public:
	Dialogue *dialogue;
	int lineIndex;
	DialogueFileView *fileView;
	AddDialogueOptionWindow();
};

#endif