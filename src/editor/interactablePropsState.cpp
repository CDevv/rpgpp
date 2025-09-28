#include "interactablePropsState.hpp"
#include <cstring>

InteractablePropsState::InteractablePropsState() 
{
	strcpy(diagText, "");
}

void InteractablePropsState::setDefaults(Interactable *interactable)
{
	if (interactable == nullptr) return;

	switch (interactable->getType()) {
	case INT_TWO:
		this->dialogue = (static_cast<InteractableTwo*>(interactable))->getDialogue();
		std::strcpy(diagText, 
			(static_cast<InteractableTwo*>(interactable))->getDialogue().lines.at(0).text.c_str());
		break;
	default:
		break;
	}
}

void InteractablePropsState::updateProps(Interactable *interactable)
{
	if (interactable == nullptr) return;

	Dialogue dialogue;
	switch (interactable->getType()) {
	case INT_TWO:
		dialogue.lines.push_back(DialogueLine {
			diagText
		});

        (static_cast<InteractableTwo*>(interactable))->setDialogue(dialogue);
		break;
	default:
		break;
	}
}

void InteractablePropsState::setDialogue(Dialogue dialogue)
{
	this->dialogue = dialogue;
}

Dialogue InteractablePropsState::getDialogue()
{
	return dialogue;
}