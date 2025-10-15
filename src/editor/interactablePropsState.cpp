#include "interactablePropsState.hpp"
#include "dialogueBalloon.hpp"
#include "interactable.hpp"
#include <cstring>

InteractablePropsState::InteractablePropsState()
{
	strcpy(diagText, "");
}

void InteractablePropsState::setDefaults(IntBaseWrapper *interactable)
{
    if (interactable == nullptr) return;

    IntBase<Dialogue>* diag;
    switch (interactable->type) {
	case INT_TWO:
		diag = static_cast<IntBase<Dialogue>*>(interactable);
		this->dialogue = diag->get();
		if (!dialogue.lines.empty()) {
		    strcpy(diagText, dialogue.lines.at(0).text.c_str());
		} else {
		    strcpy(diagText, "Empty dialogue!");
		}
		break;
	default:
		break;
	}
}

void InteractablePropsState::updateProps(IntBaseWrapper *interactable)
{
	if (interactable == nullptr) return;

	Dialogue dialogue;
	switch (interactable->type) {
	case INT_TWO:
		dialogue.lines.push_back(DialogueLine {
			diagText
		});

        (static_cast<IntBase<Dialogue>*>(interactable))->set(dialogue);
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
