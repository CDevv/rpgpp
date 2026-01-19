#include "screens/projectScreen.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"

void screens::ProjectScreen::initItems(tgui::Group::Ptr layout) {
    auto project = Editor::instance->getProject();

    auto label = tgui::Label::create("text.");

    if (project == nullptr) {
        label->setText("uhh, nullptr");
    } else {
        label->setText(project->getTitle());
    }

    layout->add(label);
}