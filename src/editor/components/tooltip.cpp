#include "components/tooltip.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Widgets/Label.hpp"
#include <memory>

Tooltip::Tooltip(const std::string& text) : tgui::Label() {
    this->setText(text);
    // TODO: should probably not be hard coded.
    this->getRenderer()->setBackgroundColor(tgui::Color::Black);
    this->getRenderer()->setTextSize(16);
}

Tooltip::Ptr Tooltip::create(const std::string& text) {
    return std::make_shared<Tooltip>(text);
}

Tooltip::Ptr Tooltip::copy(ConstPtr widget) {
    if (widget) {
        return std::static_pointer_cast<Tooltip>(widget->clone());
    }
    return nullptr;
}

tgui::Widget::Ptr Tooltip::clone() const {
    return std::make_shared<Tooltip>(*this);
}
