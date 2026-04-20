
#include "childWindows/settingsPanel/general.hpp"

#include <TGUI/Widgets/MessageBox.hpp>
#include <cstdio>
#include <cstdlib>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "raylib.h"
#include "winapi.hpp"

constexpr const int ANIMATION_DURATION = 200;

SettingsPanelGeneral::SettingsPanelGeneral(tgui::TabContainer::Ptr tabContainer)
	: SettingsPanelBase(std::move(tabContainer), "screen.options.general._label") {
	TranslationService &ts = Editor::instance->getTranslations();
	ThemeService &theme = Editor::instance->getThemeService();

	const tgui::ScrollablePanel::Ptr scrollPanel = tgui::ScrollablePanel::create();
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
	for (const auto &[name, key] : ts.langKeyToName) {
		languageSelector->addItem(key, name);
	}

	if (const auto langTranslation = ts.getKey("language"); languageSelector->contains(langTranslation))
		languageSelector->setSelectedItem(langTranslation);

	languageSelector->onItemSelect.connect([&](const tgui::String &item, const tgui::String &id) {
		ConfigurationService &configService = Editor::instance->getConfiguration();
		ts.setLanguage(ts.getLanguageIdentifierByKey(item.toStdString()));
		configService.setStringValue("language", id.toStdString());
		configService.saveConfiguration();

		// Can't think of a way to reload the menu bar without recreating it
		Editor::instance->getGui().initMenuBar();
		if (const auto ptr = Editor::instance->getGui().menuBar.lock()) {
			Editor::instance->getGui().currentScreen->bindMenuBarAndHK(ptr);
		}
	});

	languageLayout->setSize({"100%", 30});
	const auto languageLabel = tgui::Label::create();
	languageLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	bindTranslation(languageLabel, "screen.options.general.language", &tgui::Label::setText);

	languageLayout->add(languageLabel);
	languageLayout->add(languageSelector);

	// Theme
	const auto themeLayout = tgui::HorizontalLayout::create();
	const auto themeSelector = tgui::ComboBox::create();

	themeLayout->setSize({"100%", 30});
	const auto themeLabel = tgui::Label::create();
	themeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	bindTranslation(themeLabel, "screen.options.general.theme", &tgui::Label::setText);

	for (const auto &themeName : theme.getThemes()) themeSelector->addItem(themeName);

	themeSelector->setSelectedItem(theme.current_theme_name);

	this->promptUserBox = tgui::MessageBox::create("");
	bindTranslation<tgui::MessageBox>(this->promptUserBox, "screen.options.general.theme", &tgui::MessageBox::setTitle);
	bindTranslation(promptUserBox, "screen.options.general.theme_notice", &tgui::MessageBox::setText);
	bindTranslationWithCallback<tgui::MessageBox>(this->promptUserBox, [&](std::shared_ptr<tgui::MessageBox> box,
																		   TranslationService &ts) {
		box->changeButtons(std::vector<tgui::String>{ts.getKey("button.restart"), ts.getKey("button.restart_later")});
	});

	this->promptUserBox->onButtonPress.connect([&](const tgui::String &text) {
		if (text == ts.getKey("button.restart")) {
			ChangeDirectory(GetApplicationDirectory());
#ifdef __linux__

			if (const FILE *handle = popen("./editor", "r"); handle == nullptr) {
				fprintf(stderr, "failed to relaunch editor..\n");
				return;
			}
#endif

#ifdef _WIN32
			if (!WinCreateDetachedExecutable("editor.exe")) {
				fprintf(stderr, "failed to relaunch editor..\n");
				return;
			}
#endif

			exit(0);
		}
		Editor::instance->getGui().gui->remove(promptUserBox);
	});

	themeSelector->onItemSelect.connect([&](const tgui::String &item) {
		ConfigurationService &configService = Editor::instance->getConfiguration();
		configService.setStringValue("theme", item.toStdString());
		configService.saveConfiguration();

		promptUserBox->setOrigin({0.5, 0.5});
		promptUserBox->setPosition({"50%", "50%"});

		Editor::instance->getGui().gui->add(promptUserBox);
	});

	themeLayout->add(themeLabel);
	themeLayout->add(themeSelector);

	layout->add(languageLayout);
	layout->add(themeLayout);

	scrollPanel->add(layout);
	panel->add(scrollPanel);
}
