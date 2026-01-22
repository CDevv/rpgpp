#include "welcomeScreen.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "editorGuiService.hpp"
#include "translationService.hpp"

void screens::WelcomeScreen::initItems(tgui::Group::Ptr layout) {
	auto &ts = Editor::instance->getTranslations();

	const auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->setPosition({"50%", "50%"});
	verticalLayout->setWidth(640);
	verticalLayout->setOrigin(0.5, 0.5);
	verticalLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	EditorGuiService::createLogoCenter(verticalLayout);
	// TODO: Make the menu, actually get you started.
	// The "Get Started!" text.

	const auto headerLabel =
		tgui::Label::create(ts.getKey("screen.starting.get_started"));
	headerLabel->getRenderer()->setTextSize(32);
	headerLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	verticalLayout->add(headerLabel);

	const auto introLabel =
		tgui::Label::create(ts.getKey("screen.starting.description"));
	introLabel->getRenderer()->setTextSize(16);
	introLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	introLabel->setSize({0, 81});
	verticalLayout->add(introLabel);

	const auto newProjButton = tgui::Button::create(ts.getKey("file.new_project"));
	newProjButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);
	const auto openProjButton = tgui::Button::create(ts.getKey("file.open_project"));
	openProjButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);

	openProjButton->onPress([] {
		Editor::instance->getFs().promptOpenProject();
	});

	verticalLayout->add(newProjButton);
	verticalLayout->add(openProjButton);

	layout->add(verticalLayout);
}