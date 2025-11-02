#include "interactablePropsState.hpp"
#include "dialogueBalloon.hpp"
#include "interactable.hpp"

InteractablePropsState::InteractablePropsState()
{
    onTouch = false;
    diagSource = "";
}

void InteractablePropsState::setDefaults(IntBaseWrapper *interactable)
{
    if (interactable == nullptr) return;

    onTouch = interactable->onTouch;

    IntBase<DiagInt>* diag;
    switch (interactable->type) {
	case INT_TWO:
		diag = static_cast<IntBase<DiagInt>*>(interactable);
		diagSource = diag->get().dialogueSource;
		break;
	case INT_WARPER:
        diagSource = (static_cast<IntBase<WarperInt>*>(interactable))->get().targetRoom;
	    break;
	default:
		break;
	}
}

void InteractablePropsState::updateProps(IntBaseWrapper *interactable)
{
	if (interactable == nullptr) return;

	interactable->setOnTouch(onTouch);

	DiagInt dialogue;
	switch (interactable->type) {
	case INT_TWO:
		dialogue.dialogueSource = diagSource;

        (static_cast<IntBase<DiagInt>*>(interactable))->set(dialogue);
		break;
	case INT_WARPER:
	    (static_cast<IntBase<WarperInt>*>(interactable))->set({diagSource});
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
