//
// Created by thefirey33 on 21.01.2026.
//

#include "editorOptionsScreen.h"

#include "editor.hpp"
#include "translationService.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"

namespace screens {
    void EditorOptionsScreen::initItems(const tgui::Group::Ptr layout) {
        TranslationService &ts = Editor::instance->getTranslations();
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
        const auto languageLabel = tgui::Label::create(ts.getKey("options.language"));
        languageLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);

        languageLayout->add(languageLabel);
        languageLayout->add(languageSelector);

        verticGrowLayout->add(languageLayout);
        verticGrowLayout->add(backButton);

        layout->add(verticGrowLayout);
    }
}
