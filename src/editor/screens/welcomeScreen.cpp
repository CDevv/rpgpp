#include "welcomeScreen.hpp"
#include "TGUI/Widgets/BoxLayout.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Picture.hpp"
#include "TGUI/Widgets/VerticalLayout.hpp"
#include "editor.hpp"
#include "editorGuiService.hpp"
#include "projectScreen.hpp"
#include "translationService.hpp"
#include <memory>

void screens::WelcomeScreen::initItems(tgui::Group::Ptr layout) {
	auto &ts = Editor::instance->translationService;

	auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->setPosition({"50%", "50%"});
	verticalLayout->setWidth(640);
	verticalLayout->setOrigin(0.5, 0.5);
	verticalLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	auto boxLayout = tgui::BoxLayout::create({"100%", 96});
	auto welcomePic = tgui::Picture::create("resources/logo-ups.png");
	welcomePic->setOrigin({0.5, 0.5});
	welcomePic->setPosition({"50%", "50%"});
	boxLayout->add(welcomePic);
	verticalLayout->add(boxLayout);

	// TODO: Make the menu, actually get you started.
	// The "Get Started!" text.

	auto headerLabel =
	    tgui::Label::create(ts->getKey("screen.starting.get_started"));
	headerLabel->getRenderer()->setTextSize(32);
	headerLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	verticalLayout->add(headerLabel);

	auto introLabel =
	    tgui::Label::create(ts->getKey("screen.starting.description"));
	introLabel->getRenderer()->setTextSize(16);
	introLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	introLabel->setSize({0, 81});
	verticalLayout->add(introLabel);

	auto newProjButton =
	    tgui::Button::create(ts->getKey("file.new_project"));
	newProjButton->getRenderer()->setTextSize(16);
	auto openProjButton =
	    tgui::Button::create(ts->getKey("file.open_project"));
	openProjButton->getRenderer()->setTextSize(16);

	openProjButton->onPress([] {
		Editor::instance->guiService->setScreen(std::make_unique<ProjectScreen>());
	});

	verticalLayout->add(newProjButton);
	verticalLayout->add(openProjButton);

	layout->add(verticalLayout);
}