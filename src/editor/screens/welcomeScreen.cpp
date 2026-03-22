#include "screens/welcomeScreen.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "bindTranslation.hpp"
#include "editor.hpp"
#include "project.hpp"
#include "services/editorGuiService.hpp"
#include "services/translationService.hpp"
#include "widgets/newProjectWindow.hpp"
#include <memory>

void screens::WelcomeScreen::initItems(tgui::Group::Ptr layout) {
	auto &ts = Editor::instance->getTranslations();

	if (!Editor::instance->getGui().menuBar.expired()) {
		auto menuBarPtr = Editor::instance->getGui().menuBar.lock();
		menuBarPtr->setMenuItemEnabled(
			{ts.getKey("menu.file._label"), ts.getKey("menu.file.save_file")},
			false);
		menuBarPtr->setMenuItemEnabled(
			{ts.getKey("menu.edit._label"), ts.getKey("menu.edit.undo")},
			false);
		menuBarPtr->setMenuItemEnabled(
			{ts.getKey("menu.edit._label"), ts.getKey("menu.edit.redo")},
			false);
	}

	const auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->setPosition({"50%", "50%"});
	verticalLayout->setWidth(640);
	verticalLayout->setOrigin(0.5, 0.5);
	verticalLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	EditorGuiService::createLogoCenter(verticalLayout);

	const auto headerLabel = tgui::Label::create("");
	bindTranslation<tgui::Label>(headerLabel, "screen.starting.get_started",
								 &tgui::Label::setText);
	headerLabel->setTextSize(32);
	headerLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	verticalLayout->add(headerLabel);

	const auto introLabel = tgui::Label::create("");
	bindTranslation<tgui::Label>(introLabel, "screen.starting.description",
								 &tgui::Label::setText);
	introLabel->setTextSize(16);
	introLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	introLabel->setSize({0, 81});
	verticalLayout->add(introLabel);

	const auto newProjButton = tgui::Button::create();
	bindTranslation<tgui::Button>(newProjButton, "menu.file.new_project",
								  &tgui::Button::setText);
	newProjButton->setTextSize(ACTION_BUTTON_SIZE);
	const auto openProjButton = tgui::Button::create();
	bindTranslation<tgui::Button>(openProjButton, "menu.file.open_project",
								  &tgui::Button::setText);
	openProjButton->setTextSize(ACTION_BUTTON_SIZE);

	newProjButton->onPress(
		[this] { Editor::instance->getFs().promptNewProject(); });

	openProjButton->onPress(
		[] { Editor::instance->getFs().promptOpenProject(); });

	verticalLayout->add(newProjButton);
	verticalLayout->add(openProjButton);

	layout->add(verticalLayout);
}
