#include "screens/projectScreen.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalWrap.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "raylib.h"

void screens::ProjectScreen::initItems(tgui::Group::Ptr layout) {
    auto toolBar = createToolBar();
    auto panel = createResourcesList();

    layout->add(panel);
    layout->add(toolBar);

    // Maximize when a project is opened
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
}

tgui::HorizontalWrap::Ptr screens::ProjectScreen::createToolBar() {
    auto toolBar = tgui::HorizontalWrap::create({ "100%", 54 });
    toolBar->setPosition(0, 0);
    toolBar->getRenderer()->setSpaceBetweenWidgets(8.0f);
    toolBar->getRenderer()->setPadding(8);

    auto barSize = toolBar->getSize().y;

    auto project = Editor::instance->getProject();

    auto label = tgui::Label::create("text.");
    label->setVerticalAlignment(tgui::VerticalAlignment::Center);
    label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    label->setTextSize(16);
    label->setSize(256, "100%");

    if (project == nullptr) {
        label->setText("uhh, nullptr");
    } else {
        label->setText(project->getTitle());
    }

    toolBar->add(label);

    auto playBtn = tgui::BitmapButton::create();
    auto playtestImg = tgui::Texture("resources/playtest.png");
    playBtn->setImage(playtestImg);
    playBtn->setSize({ barSize, "100%" });
    toolBar->add(playBtn);

    auto buildBtn = tgui::BitmapButton::create();
    auto buildImg = tgui::Texture("resources/build.png");
    buildBtn->setImage(buildImg);
    buildBtn->setSize({ barSize, "100%" });
    toolBar->add(buildBtn);

    return toolBar;
}

tgui::Group::Ptr screens::ProjectScreen::createResourcesList() {
    auto project = Editor::instance->getProject();

    auto group = tgui::Group::create({"264", "100% - 54"});
    group->setPosition(0, 54);

    auto resourceChoose = tgui::ComboBox::create();
    resourceChoose->setPosition(0, 0);
    resourceChoose->setSize("100%", 54);
    resourceChoose->addMultipleItems({"TileSets", "Maps"});
    resourceChoose->setSelectedItem("Maps");

    group->add(resourceChoose);

    auto panel = tgui::ScrollablePanel::create({"100%", "100% - 54"});
    panel->setPosition(0, 54);
    panel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
    panel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
    panel->getRenderer()->setBackgroundColor(tgui::Color::applyOpacity(tgui::Color::Black, 0.2));

    group->add(panel);

    auto vertLayout = tgui::GrowVerticalLayout::create();
    panel->add(vertLayout);

    resourceChoose->onItemSelect([project, vertLayout] (int index) {
        vertLayout->removeAllWidgets();

        EngineFileType currentFileType = static_cast<EngineFileType>(index);
        for (auto filePath : project->getPaths(currentFileType)) {
            auto fileBtn = tgui::Button::create(GetFileName(filePath.c_str()));
            fileBtn->setSize("100%", 36);
            vertLayout->add(fileBtn);
        }
    });

    if (project != nullptr) {
        EngineFileType currentFileType = static_cast<EngineFileType>(resourceChoose->getSelectedItemIndex());
        for (auto filePath : project->getPaths(currentFileType)) {
            auto fileBtn = tgui::Button::create(GetFileName(filePath.c_str()));
            fileBtn->setSize("100%", 36);
            vertLayout->add(fileBtn);
        }
    }

    return group;
}