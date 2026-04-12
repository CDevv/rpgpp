#include "childWindows/projectSettingsPanel/game.hpp"

#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/settingsPanel/base.hpp"

ProjectSettingsPanelGame::ProjectSettingsPanelGame(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "screen.project_options.game._label") {
	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}