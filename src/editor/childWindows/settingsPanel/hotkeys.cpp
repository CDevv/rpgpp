#include "childWindows/settingsPanel/hotkeys.hpp"

#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "widgets/hotkeyModifier.hpp"

SettingsPanelHotkeys::SettingsPanelHotkeys(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "screen.options.hotkeys._label") {
	HotkeyService &hks = Editor::instance->getHotkeyService();
	TranslationService &ts = Editor::instance->getTranslations();

	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	for (auto &[k, v] : hks.listHotkeys()) {
		const auto container = tgui::HorizontalLayout::create();
		const auto hotkeyLabel = tgui::Label::create();
		hotkeyLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
		hotkeyLabel->setText(k);
		bindTranslation(hotkeyLabel, "screen.options.hotkeys." + k, &tgui::Label::setText);

		const auto hotkeyModifier = HotkeyModifier::create();

		hotkeyModifier->setKey(k, v.key, v.shift, v.ctrl, v.alt, v.super, true);
		hotkeyModifier->onChange([&](const std::string &id, Hotkey hk) {
			ConfigurationService &cfgs = Editor::instance->getConfiguration();
			hks.removeHotkey(id);
			hks.addHotkey(id, hk);
			cfgs.setStringValue("hotkeys", id, to_string(HotkeyService::pack(hk)));
			cfgs.saveConfiguration();
		});

		container->setSize({"100%", 32});
		container->add(hotkeyLabel);
		container->add(hotkeyModifier);
		layout->add(container);
	}

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}
