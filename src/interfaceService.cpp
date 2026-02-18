#include "interfaceService.hpp"
#include "colorRect.hpp"
#include "imageRect.hpp"
#include "interfaceView.hpp"
#include "resourceService.hpp"
#include "textArea.hpp"
#include <raylib.h>

InterfaceService::InterfaceService() {
	fpsVisible = false;

	this->font = LoadFontEx("LanaPixel.ttf", 13, nullptr, 250);

	Image img = LoadImage("ui-npatch.png");
	ImageResizeNN(&img, img.width * 3, img.height * 3);

	this->uiTexture = LoadTextureFromImage(img);

	Rectangle destRec =
		Rectangle{0, 0, static_cast<float>(GetScreenWidth() - 20), 140};
	destRec.x = (GetScreenWidth() - destRec.width) / 2;
	destRec.y = (GetScreenHeight() - destRec.height) - 20;
	this->dialogue = DialogueBalloon(destRec);

	this->views = std::make_unique<std::map<std::string, InterfaceView>>();

	Rectangle screenRect = Rectangle{0, 0, static_cast<float>(GetScreenWidth()),
									 static_cast<float>(GetScreenHeight())};
	InterfaceView view = InterfaceView(screenRect);

	TextArea *tArea = new TextArea(Rectangle{0, 0, 300, 200});
	tArea->setText("Text!");
	view.addElement(tArea);

	ColorRect *colorRect = new ColorRect(Rectangle{0, 50, 50, 75});
	colorRect->setColor(RED);
	view.addElement(colorRect);

	ImageRect *imageRect = new ImageRect(Rectangle{50, 50, 50, 50});
	imageRect->setTexture(LoadTexture("logo-sq.png"));
	view.addElement(imageRect);

	views->emplace("test", std::move(view));
}

InterfaceService::~InterfaceService() {
	// UnloadFont(font);
	// UnloadTexture(this->uiTexture);
}

Font InterfaceService::getFont() const { return font; }

Texture InterfaceService::getTexture() const { return uiTexture; }

void InterfaceService::showDialogue(const Dialogue &dialogue) {
	this->dialogue.showDialogue(dialogue);
}

void InterfaceService::update() {
	if (IsKeyPressed(KEY_Q)) {
		fpsVisible = !fpsVisible;
	}

	dialogue.update();

	for (auto &&item : *views) {
		item.second.update();
	}
}

void InterfaceService::draw() {
	if (fpsVisible) {
		DrawFPS(10, 10);
		DrawTextEx(font, "rpgpp", Vector2{10, 36},
				   static_cast<float>(font.baseSize), 2, RED);
	}

	dialogue.draw();

	for (auto &&item : *views) {
		item.second.draw();
	}
}

void InterfaceService::unload() const {
	UnloadFont(font);
	UnloadTexture(this->uiTexture);
}
