#ifndef _RPGPP_EDITOR_INTERACTABLEPROPSSTATE_H
#define _RPGPP_EDITOR_INTERACTABLEPROPSSTATE_H

#include "interactable.hpp"
#include "dialogueBalloon.hpp"

class InteractablePropsState {
private:
	Dialogue dialogue;
public:
	InteractablePropsState();
	void setDefaults(Interactable* interactable);
	Dialogue getDialogue();
};

#endif