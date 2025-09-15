#include "interactablePropsState.hpp"

InteractablePropsState::InteractablePropsState() {}

void InteractablePropsState::setDefaults(Interactable *interactable)
{
	if (interactable == nullptr) return;

	switch (interactable->getType()) {
	case INT_TWO:
		this->dialogue = (static_cast<InteractableTwo*>(interactable))->getDialogue();
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