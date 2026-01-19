#include "editorGuiService.hpp"
#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "aboutScreen.h"
#include "editor.hpp"
#include "guiScreen.hpp"
#include "projectScreen.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include "welcomeScreen.hpp"
#include <TGUI/AllWidgets.hpp>
#include <cmath>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

/*
	Properties of the actual window.
*/
constexpr int BASE_WINDOW_WIDTH = 800;
constexpr int BASE_WINDOW_HEIGHT = 600;
/*
	Properties of the gradient.
*/
constexpr float GRADIENT_SPEED_MUTLIPLIER = 0.3f;
constexpr float GRADIENT_COLOR_MULTIPLIER = 40.0f;

EditorGuiService::EditorGuiService() {
	reset_gui_r = false;
	gui = std::unique_ptr<tgui::Gui>();
	currentScreen = std::unique_ptr<UIScreen>();
}

void EditorGuiService::init() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, "RPG++ Editor");

	this->resetUi();
}

void EditorGuiService::resetUi() {
	this->reset_gui_r = false;
	// Check if we already have a gui already setup, if we do.. don't load
	// the same assets again.
	if (this->gui == nullptr) {
		this->gui = std::make_unique<tgui::Gui>();
		Editor::instance->setAppIcon(RPGPP_EXECUTABLE_LOGO);
	}
	tgui::Theme::setDefault(CURRENT_TESTING_THEME);

	this->setScreen(std::make_unique<screens::WelcomeScreen>(), true);
}

void EditorGuiService::uiLoop() {
	auto const &cg = this->gui;
	SetTraceLogLevel(LOG_WARNING);
	// main loop.
	while (!WindowShouldClose()) {
		cg->handleEvents();
		while (int pressed_char = GetCharPressed())
			cg->handleCharPressed(pressed_char);
		while (int pressedKey = GetKeyPressed())
			cg->handleKeyPressed(pressedKey);

		BeginDrawing();
		ClearBackground(DARKGRAY);
		// Achieve that effect of the gradient.
		auto topGradientColor = static_cast<unsigned char>(
			abs(sin(GetTime() * GRADIENT_SPEED_MUTLIPLIER)) *
			GRADIENT_COLOR_MULTIPLIER);
		// Draw the gradient.
		DrawRectangleGradientV(
			0, 0, GetRenderWidth(), GetRenderHeight(),
			{topGradientColor, topGradientColor, topGradientColor, 255},
			{40, 40, 40, 255});
		cg->draw();
		// Due to many reasons, one time... Thefirey33 decided to talk
		// to the C++ MSVC Compiler if he can reset the current state of
		// his world at any time. MSVC replied, "MEMORY EXCEPTION".
		// Thefirey33 cried, pleaded... his niko-like eyes looking at
		// MSVC. But the solution was... to put a boolean to tell the
		// loop to reset it with an if statement. NOTE: If you think of a
		// better solution. Too Bad!
		if (reset_gui_r)
			this->resetUi();
		EndDrawing();
	}
	currentScreen.reset();
	gui.reset();
}

void EditorGuiService::setScreen(std::unique_ptr<UIScreen> setToScreen,
								 bool forceSwitch) {
	if (this->currentScreen != nullptr &&
		setToScreen->getNameOfScreen() ==
			this->currentScreen->getNameOfScreen() &&
		!forceSwitch) {
		return;
	}
	auto group = this->uiChangePreInit(setToScreen.get());
	this->prevScreen = this->currentScreen.release();
	this->currentScreen.swap(setToScreen);
	this->currentScreen->initItems(group);
	gui->add(group);
}

tgui::Group::Ptr EditorGuiService::uiChangePreInit(UIScreen *setToScreen) {
	gui->removeAllWidgets();
	initMenuBar();

	auto group = tgui::Group::create({"100%"});
	group->setPosition(0, "30");

	if (this->currentScreen != nullptr)
		this->currentScreen->unloadScreen();
	std::string title = "RPG++ Editor - ";
	title.append(setToScreen->getNameOfScreen());
	SetWindowTitle(title.c_str());
	return group;
}

void EditorGuiService::setScreen(UIScreen *setToScreen, bool forceSwitch) {
	if (this->currentScreen != nullptr &&
		setToScreen->getNameOfScreen() ==
			this->currentScreen->getNameOfScreen() &&
		!forceSwitch) {
		return;
	}
	auto group = this->uiChangePreInit(setToScreen);
	this->prevScreen = this->currentScreen.release();
	this->currentScreen = std::unique_ptr<UIScreen>(setToScreen);
	this->currentScreen->initItems(group);
	gui->add(group);
}

void EditorGuiService::createLogoCenter(
	tgui::GrowVerticalLayout::Ptr verticalLayout) {

	auto boxLayout = tgui::BoxLayout::create({"100%", 96});
	auto welcomePic = tgui::Picture::create("resources/logo-ups.png");
	welcomePic->setOrigin({0.5, 0.5});
	welcomePic->setPosition({"50%", "50%"});
	boxLayout->add(welcomePic);

	verticalLayout->add(boxLayout);
}

void EditorGuiService::reloadUi() {
	auto group = this->uiChangePreInit(this->currentScreen.get());
	this->currentScreen->initItems(group);
	gui->add(group);
}

void EditorGuiService::initMenuBar() {
	// Clear if there's data left over.
	this->translations.clear();
	auto menuBar = tgui::MenuBar::create();
	auto &ts = Editor::instance->getTranslations();
	const auto &fileOptionsTranslation = ts.getKey("file.options");
	const auto &fileOpenProjectTranslation = ts.getKey("file.open_project");
	// TODO: File/Project Options.
	menuBar->addMenu(fileOptionsTranslation);
	menuBar->addMenuItem(ts.getKey("file.new_project"));
	menuBar->addMenuItem(fileOpenProjectTranslation);
	// Translation Options.
	const auto currentMenuText = ts.getKey("translations.options");
	menuBar->addMenu(currentMenuText);
	for (auto const &[language_file_name, data] : ts.translations) {
		const auto menuItemText = ts.getKey("language", language_file_name);
		menuBar->addMenuItem(menuItemText);
		this->translations.try_emplace(menuItemText, language_file_name);
	}
	menuBar->onMenuItemClick.connect(
		[this, &ts](const tgui::String &button_text) {
			if (const auto it = this->translations.find(button_text);
				it != this->translations.end()) {
				ts.current_language = it->second;
				this->reloadUi();
			}
		});

	menuBar->connectMenuItem(
		{fileOptionsTranslation, fileOpenProjectTranslation}, [] {
			auto files = tgui::FileDialog::create();

			files->onFileSelect([](const tgui::String &filePath) {
				Editor::instance->setProject(filePath.toStdString());
				Editor::instance->getGui().setScreen(
					std::make_unique<screens::ProjectScreen>(), false);
			});

			Editor::instance->getGui().gui->add(files);
		});
	const auto &aboutOptions = ts.getKey("about.options");
	const auto &aboutRpgpp = ts.getKey("about.options.rpgpp");
	menuBar->addMenu(aboutOptions);
	menuBar->addMenuItem(aboutRpgpp);

	menuBar->connectMenuItem({aboutOptions, aboutRpgpp}, [] {
		Editor::instance->getGui().setScreen(
			std::make_unique<screens::AboutScreen>(), false);
	});
	menuBar->setSize({"100%", "30"});
	this->gui->add(menuBar);
}
