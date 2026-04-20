#ifndef _RPGPP_CHILDWINDOWS_EDITDIALOGUEOPTIONWINDOW_H
#define _RPGPP_CHILDWINDOWS_EDITDIALOGUEOPTIONWINDOW_H

#include "childWindows/popupWindow.hpp"
#include "dialogue.hpp"
#include "fileViews/dialogueFileView.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/textField.hpp"

class EditDialogueOptionWindow : public PopupWindow {
public:
	Dialogue *dialogue;
	int lineIndex;
	int optionIndex;
	DialogueFileView *fileView;

	TextField::Ptr nameField;
	FileField::Ptr dialogueField;

	EditDialogueOptionWindow();
	void setup(Dialogue *dialogue, int lineIndex, int optionIndex);
};

#endif