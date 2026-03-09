#include "childWindows/aboutWindow.hpp"
#include "TGUI/Config.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "editor.hpp"
#include <string>

AboutWindow::AboutWindow(const std::string &title) : PopupWindow(title) {
	auto layout = tgui::GrowVerticalLayout::create();
	layout->getRenderer()->setSpaceBetweenWidgets(4.0f);

	EditorGuiService::createLogoCenter(layout);

	auto infoText = tgui::Label::create();
	bindTranslation(infoText, "menu.about.rpgpp_description",
					&tgui::Label::setText);
	bindTranslation<tgui::ChildWindow>(this->currentWindow, "menu.about._label",
									   &tgui::ChildWindow::setTitle);
	infoText->setTextSize(20);
	infoText->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	layout->add(infoText);

	std::stringstream versionText;
	versionText << "raylib: " << RAYLIB_VERSION << "\n";
	versionText << "rpgpp: " << RPGPP_VERSION << "\n";
	versionText << "tgui: " << TGUI_VERSION_MAJOR << "." << TGUI_VERSION_MINOR;

	auto informationLabel = tgui::Label::create(versionText.str());
	informationLabel->setTextSize(15);
	informationLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);

	layout->add(informationLabel);
	layout->setOrigin({0.5, 0.5});
	layout->setPosition({"50%", "50%"});

	this->currentWindow->add(layout);
}
