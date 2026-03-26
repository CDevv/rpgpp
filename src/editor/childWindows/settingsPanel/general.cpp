#include "childWindows/settingsPanel/general.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"

SettingsPanelGeneral::SettingsPanelGeneral(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(tabContainer, "screen.options.general._label") {
	TranslationService &ts = Editor::instance->getTranslations();
	ThemeService &theme = Editor::instance->getThemeService();

	const tgui::ScrollablePanel::Ptr scrollPanel =
		tgui::ScrollablePanel::create();
	scrollPanel->setSize("100%", "100%");
	scrollPanel->getRenderer()->setPadding(4);

	const auto layout = tgui::GrowVerticalLayout::create();
	layout->setSize("80%", "100%");
	layout->setPosition({"50%", "0%"});
	layout->setOrigin({0.5, 0});
	layout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	// Language
	const auto languageLayout = tgui::HorizontalLayout::create();
	const auto languageSelector = tgui::ComboBox::create();
	for (auto [name, key] : ts.langKeyToName) {
		languageSelector->addItem(key, name);
	}

	if (const auto langTranslation = ts.getKey("language");
		languageSelector->contains(langTranslation))
		languageSelector->setSelectedItem(langTranslation);

	languageSelector->onItemSelect.connect([&](const tgui::String &item, const tgui::String &id) {
		ConfigurationService &configService =
			Editor::instance->getConfiguration();
		ts.setLanguage(ts.getLanguageIdentifierByKey(item.toStdString()));
		configService.setStringValue("language", id.toStdString());
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
	bindTranslation(languageLabel, "screen.options.general.language",
					&tgui::Label::setText);

	languageLayout->add(languageLabel);
	languageLayout->add(languageSelector);

	// Theme
	const auto themeLayout = tgui::HorizontalLayout::create();
	const auto themeSelector = tgui::ComboBox::create();

	themeLayout->setSize({"100%", 30});
	const auto themeLabel = tgui::Label::create();
	themeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	bindTranslation(themeLabel, "screen.options.general.theme", &tgui::Label::setText);

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
	bindTranslation(warnLabel, "screen.options.general.theme_notice",
					&tgui::Label::setText);
	warnLabel->setSize({"100%", 36});

	layout->add(languageLayout);
	layout->add(themeLayout);
	layout->add(warnLabel);

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}
