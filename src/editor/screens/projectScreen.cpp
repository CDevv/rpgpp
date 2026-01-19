#include "screens/projectScreen.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"

void screens::ProjectScreen::initItems(tgui::Group::Ptr layout) {
    auto label = tgui::Label::create("text.");

    layout->add(label);
}