#include "interfaceService.hpp"

#include <raylib.h>

#include <cstdio>
#include <memory>
#include <stdexcept>

#include "button.hpp"
#include "colorRect.hpp"
#include "dialogueArea.hpp"
#include "dialogueBalloon.hpp"
#include "game.hpp"
#include "imageRect.hpp"
#include "interfaceView.hpp"
#include "ninePatchImageRect.hpp"
#include "textArea.hpp"
#include "uiElement.hpp"

InterfaceService::InterfaceService() {
	fpsVisible = false;

	// this->font = LoadFontEx("fonts/LanaPixel.ttf", 13, nullptr, 250);
	this->font = Game::getResources().getFont("LanaPixel");

	Image img = LoadImage("images/ui-npatch.png");
	ImageResizeNN(&img, img.width * 3, img.height * 3);

	this->uiTexture = LoadTextureFromImage(img);

	Rectangle destRec = Rectangle{0, 0, static_cast<float>(GetScreenWidth() - 20), 140};
	destRec.x = (GetScreenWidth() - destRec.width) / 2;
	destRec.y = (GetScreenHeight() - destRec.height) - 20;
	this->dialogue = DialogueBalloon(destRec);

	this->views = std::map<std::string, std::unique_ptr<InterfaceView>>{};

	Rectangle screenRect = Rectangle{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
	auto view = std::make_unique<InterfaceView>(screenRect);

	TextArea *tArea = new TextArea(Rectangle{0, 0, 300, 200});
	tArea->setText("Text!");
	view->addElement("tArea", tArea);

	ColorRect *colorRect = new ColorRect(Rectangle{0, 50, 50, 75});
	colorRect->setColor(RED);
	view->addElement("colorRect", colorRect);

	ImageRect *imageRect = new ImageRect(Rectangle{50, 50, 50, 50});
	imageRect->setTexture(LoadTexture("logo-sq.png"));
	view->addElement("imageRect", imageRect);

	NinePatchImageRect *npatch = new NinePatchImageRect(Rectangle{0, 200, 150, 100});
	npatch->setSource("ui-npatch.png");
	npatch->npatchInfo.top = 3;
	npatch->npatchInfo.left = 3;
	npatch->npatchInfo.bottom = 3;
	npatch->npatchInfo.right = 3;
	npatch->npatchInfo.layout = NPATCH_NINE_PATCH;
	view->addElement("npatch", npatch);

	DialogueBin testDialogue;
	DialogueLine diagLine;
	diagLine.sections.push_back({"", "Hello!"});
	testDialogue.lines.push_back(diagLine);
	DialogueArea *dialogue = new DialogueArea();
	dialogue->setRect({0, 200, 300, 50});
	dialogue->setDialogue(testDialogue);
	view->addElement("zdiag", dialogue);

	Button *button = new Button();
	button->setRect(Rectangle{0, 200, 150, 50});
	button->setText("Hello!");
	button->colorRect.borderWidth = 2;
	button->setDownElement("button2");
	button->setCallback(CALLBACK_TRIGGER, [] { printf("button1 tirggered..\n"); });
	view->addElement("button1", button);
	view->changeFocusedElement("button1");

	Button *button2 = new Button();
	button2->setRect(Rectangle{0, 250, 150, 50});
	button2->setText("Hello!");
	button2->colorRect.borderWidth = 2;
	button2->setUpElement("button1");
	button->setCallback(CALLBACK_TRIGGER, [] { printf("button2 tirggered..\n"); });
	view->addElement("button2", button2);

	// std::string viewDump = view->dumpJson().dump();
	// SaveFileText("ui.json", viewDump.c_str());

	views.emplace("test", std::move(view));

	currentViewName = "test";
}

InterfaceService::~InterfaceService() {
	// UnloadFont(font);
	// UnloadTexture(this->uiTexture);
}

Font InterfaceService::getFont() const { return font; }

Texture InterfaceService::getTexture() const { return uiTexture; }

void InterfaceService::showDialogue(const std::string &id) {
	if (Game::getBin().dialogues.count(id) > 0) {
		auto diag = Game::getBin().dialogues[id];
		showDialogue(diag);
	} else {
		throw std::runtime_error(TextFormat("This Dialogue does not exist: %s", id.c_str()));
	}
}

void InterfaceService::showDialogue(const DialogueBin &dialogue) { this->dialogue.showDialogue(dialogue); }

InterfaceView *InterfaceService::getCurrentView() {
	if (views.count(currentViewName) > 0) {
		return views.at(currentViewName).get();
	} else {
		return nullptr;
	}
}

void InterfaceService::update() {
	if (IsKeyPressed(KEY_Q)) {
		fpsVisible = !fpsVisible;
	}

	if (views.count(currentViewName) > 0) {
		InterfaceView *currentView = views.at(currentViewName).get();
		auto key = GetKeyPressed();
		if (key != KEY_NULL) {
			currentView->onNotify({static_cast<KeyboardKey>(key)});
		}
	}

	dialogue.update();

	for (auto &&item : views) {
		item.second->update();
	}
}

void InterfaceService::draw() {
	if (fpsVisible) {
		DrawFPS(10, 10);
		DrawTextEx(font, "rpgpp", Vector2{10, 36}, static_cast<float>(font.baseSize), 2, RED);
	}

	dialogue.draw();

	for (auto &&item : views) {
		item.second->draw();
	}
}

void InterfaceService::unload() const { UnloadTexture(this->uiTexture); }
