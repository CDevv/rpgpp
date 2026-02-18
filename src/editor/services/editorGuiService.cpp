#include "services/editorGuiService.hpp"
#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/MessageBox.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "screens/aboutScreen.h"
#include "screens/guiScreen.hpp"
#include "screens/welcomeScreen.hpp"
#include "services/translationService.hpp"
#include "updatable.hpp"
#include <TGUI/AllWidgets.hpp>
#include <cmath>
#include <memory>
#include <string>

#include "screens/editorOptionsScreen.h"

constexpr int BASE_WINDOW_WIDTH = 800;
constexpr int BASE_WINDOW_HEIGHT = 600;
constexpr float GRADIENT_SPEED_MUTLIPLIER = 0.3f;
constexpr float GRADIENT_COLOR_MULTIPLIER = 40.0f;

EditorGuiService::EditorGuiService() {
	reset_gui_r = false;
	gui = std::unique_ptr<tgui::Gui>();
	currentScreen = std::unique_ptr<UIScreen>();
}

void EditorGuiService::init() {
	currentCursor = MOUSE_CURSOR_DEFAULT;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, "RPG++ Editor");

	this->resetUi();
}

void EditorGuiService::resetUi() {
	ThemeService &theme = Editor::instance->getThemeService();
	ConfigurationService &configService = Editor::instance->getConfiguration();

	this->reset_gui_r = false;
	// Check if we already have a gui already setup, if we do.. don't load
	// the same assets again.
	if (this->gui == nullptr) {
		this->gui = std::make_unique<tgui::Gui>();
		Editor::setAppIcon(RPGPP_EXECUTABLE_LOGO);
	}

	string configTheme;

	try {
		configTheme = configService.getStringValue("theme");
	} catch (const std::exception &e) {
		configTheme = "Dark";
	}

	theme.setTheme(configTheme);
	// tgui::Theme::setDefault(CURRENT_TESTING_THEME);

	this->setScreen(std::make_unique<screens::WelcomeScreen>(), true);
}

void EditorGuiService::uiLoop() {
	auto const &cg = this->gui;
	SetTraceLogLevel(LOG_WARNING);

	tgui::Theme::addRendererInheritanceParent("NewProjectWindow",
											  "ChildWindow");
	tgui::Theme::addRendererInheritanceParent("RoomToolbox", "Tabs");
	// main loop.
	while (!WindowShouldClose()) {
		cg->handleEvents();
		while (const int pressed_char = GetCharPressed())
			cg->handleCharPressed(pressed_char);
		while (const int pressedKey = GetKeyPressed())
			cg->handleKeyPressed(pressedKey);

		for (const auto &widget : updatableWidgets) {
			if (!widget.expired()) {
				const std::shared_ptr<IUpdatable> spt = widget.lock();
				spt->update();
			}
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			leftMouseHeld = true;
		} else {
			if (leftMouseHeld) {
				currentScreen->leftMouseReleased(GetMouseX(),
												 GetMouseY() - MENUBAR_H);
				leftMouseHeld = false;
			}
		}
		currentScreen->mouseMove(GetMouseX(), GetMouseY() - MENUBAR_H);
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
	// this->prevScreen = this->currentScreen.release();
	this->screenHistory.push_back(std::move(this->currentScreen));
	this->currentScreen.swap(setToScreen);
	this->currentScreen->initItems(group);
	gui->add(group);
}

/**
 * this function basically handles the pre-phase before the screen changes and
 * gets everything ready for the new screen.
 * @param setToScreen the screen we are currently changing to.
 * @return the group that the screen must use to position and add it's widgets.
 */
tgui::Group::Ptr EditorGuiService::uiChangePreInit(UIScreen *setToScreen) {
	gui->removeAllWidgets();
	initMenuBar();

	auto group = tgui::Group::create({"100%"});
	group->setPosition(0, MENUBAR_H);
	group->setSize({"100%", tgui::Layout("100%") - MENUBAR_H});

	if (this->currentScreen != nullptr)
		this->currentScreen->unloadScreen();
	std::string title = "RPG++ Editor - ";
	title.append(setToScreen->getNameOfScreen());
	SetWindowTitle(title.c_str());
	return group;
}

void EditorGuiService::addUpdate(std::shared_ptr<IUpdatable> widget) {
	updatableWidgets.push_back(widget);
}

void EditorGuiService::setScreen(UIScreen *setToScreen, bool forceSwitch) {
	if (this->currentScreen != nullptr &&
		setToScreen->getNameOfScreen() ==
			this->currentScreen->getNameOfScreen() &&
		!forceSwitch) {
		return;
	}
	auto group = this->uiChangePreInit(setToScreen);
	this->screenHistory.push_back(std::move(this->currentScreen));
	this->currentScreen = std::unique_ptr<UIScreen>(setToScreen);
	this->currentScreen->initItems(group);
	gui->add(group);
}

void EditorGuiService::createLogoCenter(
	const tgui::GrowVerticalLayout::Ptr &layout) {

	const auto boxLayout = tgui::BoxLayout::create({"100%", 96});
	std::string logoPath =
		Editor::instance->getFs().getResourcePath("logo-ups.png");
	const auto welcomePic = tgui::Picture::create(logoPath.c_str());
	welcomePic->setOrigin({0.5, 0.5});
	welcomePic->setPosition({"50%", "50%"});
	boxLayout->add(welcomePic);

	layout->add(boxLayout);
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
	this->menuBar = menuBar;
	auto &ts = Editor::instance->getTranslations();

	const auto &fileOptionsTranslation = ts.getKey("menu.file");
	const auto &fileOpenProjectTranslation =
		ts.getKey("menu.file.open_project");

	menuBar->addMenu(fileOptionsTranslation);
	menuBar->addMenuItem(ts.getKey("menu.file.new_project"));
	menuBar->addMenuItem(fileOpenProjectTranslation);
	menuBar->addMenuItem(ts.getKey("menu.file.save_file"));
	menuBar->connectMenuItem(
		{fileOptionsTranslation, fileOpenProjectTranslation},
		[] { Editor::instance->getFs().promptOpenProject(); });

	menuBar->addMenu(ts.getKey("menu.edit"));
	menuBar->addMenuItem(ts.getKey("menu.edit.undo"));
	menuBar->addMenuItem(ts.getKey("menu.edit.redo"));

	const auto optionsTranslation = ts.getKey("menu.options");
	const auto editorOptionsTranslation = ts.getKey("menu.options.editor");
	menuBar->addMenu(optionsTranslation);
	menuBar->addMenuItem(editorOptionsTranslation);
	menuBar->connectMenuItem(
		{optionsTranslation, editorOptionsTranslation}, [] {
			Editor::instance->getGui().setScreen(
				std::make_unique<screens::EditorOptionsScreen>(), false);
		});

	const auto &aboutOptions = ts.getKey("menu.about"),
			   &aboutRpgpp = ts.getKey("menu.about.rpgpp");
	menuBar->addMenu(aboutOptions);
	menuBar->addMenuItem(aboutRpgpp);

	menuBar->connectMenuItem({aboutOptions, aboutRpgpp}, [] {
		Editor::instance->getGui().setScreen(
			std::make_unique<screens::AboutScreen>(), false);
	});
	menuBar->setSize({"100%", MENUBAR_H});
	this->gui->add(menuBar);
}

void EditorGuiService::gotoPreviousScreen() {
	if (!this->screenHistory.empty()) {
		std::unique_ptr<UIScreen> lastScreen =
			std::move(this->screenHistory.back());
		this->screenHistory.pop_back();

		auto group = this->uiChangePreInit(lastScreen.get());
		this->currentScreen = std::move(lastScreen);
		this->currentScreen->initItems(group);
		gui->add(group);
	}
}

void EditorGuiService::centerWidget(tgui::Widget::Ptr widget) {
	widget->setPosition(
		(GetScreenWidth() / 2.0f) - (widget->getSize().x / 2.0f),
		(GetScreenHeight() / 2.0f) - (widget->getSize().y / 2.0f));
}

void EditorGuiService::alert(tgui::String title, tgui::String content) {
	tgui::MessageBox::Ptr dialog = tgui::MessageBox::create();
	dialog->setText(content);
	dialog->setTitle(title);
	dialog->addButton("Ok");
	dialog->onButtonPress([this, dialog]() {
		gui->remove(dialog);
	});
	gui->add(dialog);
	centerWidget(dialog);
}