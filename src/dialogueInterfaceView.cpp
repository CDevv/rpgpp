#include "dialogueInterfaceView.hpp"

#include <memory>

#include "button.hpp"
#include "dialogueArea.hpp"
#include "dialogueBalloon.hpp"
#include "interfaceView.hpp"
#include "ninePatchImageRect.hpp"
#include "raylib.h"
#include "uiElement.hpp"

DialogueInterfaceView::DialogueInterfaceView() {
	NinePatchImageRect *diagImage = new NinePatchImageRect(Rectangle{10, 320, 620, 140});
	diagImage->npatchInfo.top = 3;
	diagImage->npatchInfo.left = 3;
	diagImage->npatchInfo.bottom = 3;
	diagImage->npatchInfo.right = 3;
	diagImage->npatchInfo.layout = NPATCH_NINE_PATCH;
	diagImage->setScale(3);
	diagImage->setSource("ui-npatch.png");

	addElement("diagImage", diagImage, 0);

	DialogueBin testDialogue;
	DialogueLine diagLine;
	diagLine.sections.push_back({"", "Hello!"});
	testDialogue.lines.push_back(diagLine);

	DialogueArea *diagArea = new DialogueArea(Rectangle{19, 329, 611, 131});
	diagArea->setDialogue(testDialogue);

	addElement("dialogueArea", diagArea, 1);
}

DialogueInterfaceView::DialogueInterfaceView(const std::string &filePath) : InterfaceView(filePath) {}

DialogueInterfaceView::DialogueInterfaceView(InterfaceViewBin &bin) : InterfaceView(bin) {}

void DialogueInterfaceView::setDialogue(DialogueBin *dialogue) {
	this->dialogue = dialogue;
	if (elementExists("dialogueArea")) {
		auto *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
		diagArea->setDialogue(*dialogue);
	}
}

void DialogueInterfaceView::onNotify(Event event) {
	if (event.key == KEY_Z) {
		if (elementExists("dialogueArea")) {
			auto *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
			diagArea->advanceToNextLine();

			if (diagArea->line->hasOptions && elementExists("optionButton")) {
				Button *baseButton = static_cast<Button *>(getElement("optionButton"));
				Rectangle originRect = baseButton->getRect();
				int i = 0;
				for (auto &option : diagArea->line->options) {
					Button *button = new Button();
					button->setText(option.title);

					Rectangle buttonRect = {originRect.x, originRect.y + (originRect.height * i), originRect.width,
											originRect.height};
					button->setRect(buttonRect);

					auto buttonProps = button->getProps();
					for (auto &originProp : baseButton->getProps()) {
						buttonProps[originProp.first] = originProp.second;
					}

					button->setUpElement(TextFormat("option-%i", i - 1));
					button->setDownElement(TextFormat("option-%i", i + 1));

					addElement(TextFormat("option-%i", i), button, 2);

					i++;
				}
			}
		}
	}

	InterfaceView::onNotify(event);
}

void DialogueInterfaceView::update() {
	if (elementExists("dialogueArea")) {
		auto *diagArea = static_cast<DialogueArea *>(getElement("dialogueArea"));
		if (diagArea->isFinishedTyping() && diagArea->line->hasOptions) {
			getElement("optionsBg")->setVisible(true);
			int i = 0;
			for (auto &option : diagArea->line->options) {
				getElement(TextFormat("option-%i", i))->setVisible(true);
				i++;
			}
			changeFocusedElement(TextFormat("option-0"));
		}
		if (diagArea->isDialogueFinished()) {
			// finished dialogue..
		}
	}

	InterfaceView::update();
}

void DialogueInterfaceView::draw() { InterfaceView::draw(); }