//
// Created by thefirey33 on 21.01.2026.
//

#include "screens/editorOptionsScreen.h"

#include "editor.hpp"
#include "services/translationService.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"

namespace screens {
    void EditorOptionsScreen::initItems(const tgui::Group::Ptr layout) {
        TranslationService &ts = Editor::instance->getTranslations();
        ThemeService &theme = Editor::instance->getThemeService();

        const auto verticGrowLayout = tgui::GrowVerticalLayout::create();
        verticGrowLayout->setSize(640, "100%");
        verticGrowLayout->setPosition({"50%", "50%"});
        verticGrowLayout->setOrigin({0.5, 0.5});
        verticGrowLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);
        const auto topOptionsHeader = tgui::Label::create(ts.getKey("menu.options.editor"));
        topOptionsHeader->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        topOptionsHeader->getRenderer()->setTextSize(20);
        verticGrowLayout->add(topOptionsHeader);

        const auto backButton = tgui::Button::create(ts.getKey("button.go_back"));
        backButton->onPress.connect([] {
            Editor::instance->getGui().gotoPreviousScreen();
        });
        backButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);

        // Language
        const auto languageLayout = tgui::HorizontalLayout::create();
        const auto languageSelector = tgui::ComboBox::create();
        for (auto [name, key] : ts.translations) {
            if (auto languageKey = key.find("language"); languageKey != key.end())
                languageSelector->addItem(languageKey->second);
            else
                languageSelector->addItem(name);
        }
        if (const auto langTranslation = ts.getKey("language"); languageSelector->contains(langTranslation))
            languageSelector->setSelectedItem(langTranslation);

        languageSelector->onItemSelect.connect([&](const tgui::String& item) {
            ts.current_language = ts.getLanguageIdentifierByKey(item.toStdString());
            ConfigurationService &configService = Editor::instance->getConfiguration();
            configService.setStringValue("language", ts.current_language);
            configService.saveConfiguration();
            Editor::instance->getGui().reloadUi();
        });

        languageLayout->setSize({"100%", 30});
        const auto languageLabel = tgui::Label::create(ts.getKey("screen.options.language"));
        languageLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);

        languageLayout->add(languageLabel);
        languageLayout->add(languageSelector);

        // Theme
        const auto themeLayout = tgui::HorizontalLayout::create();
        const auto themeSelector = tgui::ComboBox::create();

        themeLayout->setSize({"100%", 30});
        const auto themeLabel = tgui::Label::create(ts.getKey("screen.options.theme"));
        themeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);

        for (const auto& theme : theme.getThemes())
            themeSelector->addItem(theme);

        themeSelector->setSelectedItem(theme.current_theme_name);

        themeSelector->onItemSelect.connect([&](const tgui::String& item) {
            ConfigurationService &configService = Editor::instance->getConfiguration();
            configService.setStringValue("theme", item.toStdString());
            configService.saveConfiguration();
            theme.setTheme(item.toStdString());
            Editor::instance->getGui().reloadUi();
        });

        themeLayout->add(themeLabel);
        themeLayout->add(themeSelector);

        verticGrowLayout->add(languageLayout);
        verticGrowLayout->add(themeLayout);
        verticGrowLayout->add(backButton);

        layout->add(verticGrowLayout);

    }
}
