#include "interfaceView.hpp"
#include "uiElement.hpp"
#include <memory>

InterfaceView::InterfaceView() : rect(Rectangle {}) {
}

InterfaceView::InterfaceView(Rectangle rect)
{
    this->rect = rect;
    this->elements = std::vector<std::unique_ptr<UIElement>>{};
}

void InterfaceView::addElement(UIElement* element)
{
    std::unique_ptr<UIElement> ptr = std::unique_ptr<UIElement>{};
    ptr.reset(element);
    this->elements.push_back(std::move(ptr));
}

void InterfaceView::update() const
{
    for (auto&& i : elements) {
        i->update();
    }
}

void InterfaceView::draw()
{
    for (auto&& i : elements) {
        i->draw();
    }
}
