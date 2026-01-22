#include "aboutScreen.h"
#include "TGUI/Config.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/Widgets/BoxLayout.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Picture.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <cstring>
#include <iterator>
#include <nlohmann/detail/abi_macros.hpp>
#include <sstream>
#include <string>

namespace screens {
void AboutScreen::initItems(tgui::Group::Ptr layout) {
	TranslationService &tService = Editor::instance->getTranslations();
	auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->setPosition({"50%", "50%"});
	verticalLayout->setOrigin(0.5, 0.5);
	verticalLayout->setWidth("50%");
	verticalLayout->setWidth(640);
	verticalLayout->getRenderer()->setSpaceBetweenWidgets(5.0f);
	EditorGuiService::createLogoCenter(verticalLayout);
	auto goBack = tgui::Button::create(tService.getKey("button.go_back"));
	goBack->onPress.connect([] {
		Editor::instance->getGui().setScreen(
			Editor::instance->getGui().prevScreen, false);
	});
	goBack->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);
	std::stringstream informationText;
	informationText << "raylib: " << RAYLIB_VERSION << "\n";
	informationText << "rpgpp: " << RPGPP_VERSION << "\n";
	informationText << "tgui: " << TGUI_VERSION_MAJOR << "."
					<< TGUI_VERSION_MINOR;
	auto informationLabel = tgui::Label::create(informationText.str());
	informationLabel->getRenderer()->setTextSize(20);
	informationLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	verticalLayout->add(informationLabel);
	verticalLayout->add(goBack);
	layout->add(verticalLayout);
}

void AboutScreen::unloadScreen() {}
} // namespace screens