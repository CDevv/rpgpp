#include "childWindows/settingsWindow.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "editor.hpp"
#include "widgets/hotkeyModifier.hpp"

SettingsWindow::SettingsWindow(const std::string &title) : PopupWindow(title) {
	TranslationService &ts = Editor::instance->getTranslations();
	ThemeService &theme = Editor::instance->getThemeService();
	HotkeyService &hks = Editor::instance->getHotkeyService();

	bindTranslation<tgui::ChildWindow>(this->currentWindow,
									   "menu.options._label",
									   &tgui::ChildWindow::setTitle);

	const tgui::ScrollablePanel::Ptr scrollPanel =
		tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);
	const auto layout = tgui::GrowVerticalLayout::create();

	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	const auto topOptionsHeader = tgui::Label::create();
	topOptionsHeader->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	topOptionsHeader->setTextSize(20);
	bindTranslation(topOptionsHeader, "menu.options.editor",
					&tgui::Label::setText);
	layout->add(topOptionsHeader);

	// Language
	const auto languageLayout = tgui::HorizontalLayout::create();
	const auto languageSelector = tgui::ComboBox::create();
	for (auto [name, key] : ts.translations) {
		if (auto languageKey = key.find("language"); languageKey != key.end())
			languageSelector->addItem(languageKey->second);
		else
			languageSelector->addItem(name);
	}

	if (const auto langTranslation = ts.getKey("language");
		languageSelector->contains(langTranslation))
		languageSelector->setSelectedItem(langTranslation);

	languageSelector->onItemSelect.connect([&](const tgui::String &item) {
		ConfigurationService &configService =
			Editor::instance->getConfiguration();
		ts.setLanguage(ts.getLanguageIdentifierByKey(item.toStdString()));
		configService.setStringValue("language", ts.getCurrentLanguage());
		configService.saveConfiguration();

		// Can't think of a way to reload the menu bar without recreating it
		Editor::instance->getGui().initMenuBar();
		if (auto ptr = Editor::instance->getGui().menuBar.lock()) {
			Editor::instance->getGui().currentScreen->bindMenuBarAndHK(ptr);
		}
	});

	languageLayout->setSize({"100%", 30});
	const auto languageLabel = tgui::Label::create();
	languageLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	bindTranslation(languageLabel, "screen.options.language",
					&tgui::Label::setText);

	languageLayout->add(languageLabel);
	languageLayout->add(languageSelector);

	// Theme
	const auto themeLayout = tgui::HorizontalLayout::create();
	const auto themeSelector = tgui::ComboBox::create();

	themeLayout->setSize({"100%", 30});
	const auto themeLabel = tgui::Label::create();
	themeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	bindTranslation(themeLabel, "screen.options.theme", &tgui::Label::setText);

	for (const auto &theme : theme.getThemes())
		themeSelector->addItem(theme);

	themeSelector->setSelectedItem(theme.current_theme_name);

	themeSelector->onItemSelect.connect([&](const tgui::String &item) {
		ConfigurationService &configService =
			Editor::instance->getConfiguration();
		configService.setStringValue("theme", item.toStdString());
		configService.saveConfiguration();
		theme.setTheme(item.toStdString());
		Editor::instance->getGui().reloadUi();
	});

	themeLayout->add(themeLabel);
	themeLayout->add(themeSelector);

	auto warnLabel = tgui::Label::create();
	bindTranslation(warnLabel, "screen.options.theme_notice",
					&tgui::Label::setText);
	warnLabel->setSize({"100%", 36});

	layout->add(languageLayout);
	layout->add(themeLayout);
	layout->add(warnLabel);

	// Hotkeys
	const auto hotkeyLabel = tgui::Label::create();
	bindTranslation(hotkeyLabel, "screen.options.hotkey",
					&tgui::Label::setText);

	layout->add(hotkeyLabel);

	for (auto &[k, v] : hks.listHotkeys()) {
		const auto container = tgui::HorizontalLayout::create();
		const auto hotkeyLabel = tgui::Label::create();
		hotkeyLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
		hotkeyLabel->setText(k);

		const auto hotkeyModifier = HotkeyModifier::create();

		hotkeyModifier->setKey(k, v.key, v.shift, v.ctrl, v.alt, v.super, true);
		hotkeyModifier->onChange([&](const std::string &id, Hotkey hk) {
			ConfigurationService &cfgs = Editor::instance->getConfiguration();
			hks.removeHotkey(id);
			hks.addHotkey(id, hk);
			cfgs.setStringValue("hotkeys", id,
								to_string(HotkeyService::pack(hk)));
			cfgs.saveConfiguration();
		});

		container->setSize({"100%", 32});
		container->add(hotkeyLabel);
		container->add(hotkeyModifier);
		layout->add(container);
	}

	scrollPanel->add(layout);
	this->currentWindow->add(scrollPanel);
}
