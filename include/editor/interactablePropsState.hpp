#ifndef _RPGPP_EDITOR_INTERACTABLEPROPSSTATE_H
#define _RPGPP_EDITOR_INTERACTABLEPROPSSTATE_H

#include "interactable.hpp"
#include "dialogueBalloon.hpp"

class InteractablePropsState {
private:
	Dialogue dialogue;
public:
	char diagText[256];
	InteractablePropsState();
	void setDefaults(Interactable* interactable);
	void updateProps(Interactable* interactable);
	void setDialogue(Dialogue dialogue);
	Dialogue getDialogue();
};

#endif