#include "components/resizableContainer.hpp"
#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "editorGuiService.hpp"
#include "raylib.h"
#include <memory>

bool bounded(int value, int min, int max) {
    return value >= min && value <= max;
}

ResizableContainer::ResizableContainer(const tgui::Layout2d &size, const tgui::Layout2d &position)
    : tgui::Group()
{
    this->setSize(size);
    this->setPosition(position);
}

void ResizableContainer::setMaxResizeWidth(int width) { maxResizeWidth = width; }
int ResizableContainer::getMaxResizeWidth() { return maxResizeWidth; }
void ResizableContainer::setMaxResizeHeight(int height) { maxResizeHeight = height; }
int ResizableContainer::getMaxResizeHeight() { return maxResizeHeight; }
void ResizableContainer::setMinResizeWidth(int width) { minResizeWidth = width; }
void ResizableContainer::setMinResizeHeight(int height) { minResizeHeight = height; }
int ResizableContainer::getMinResizeWidth() { return minResizeWidth; }
int ResizableContainer::getMinResizeHeight() { return minResizeHeight; }
void ResizableContainer::setGrabberSize(int size) { grabberSize = size; }
int ResizableContainer::getGrabberSize() { return grabberSize; }

void ResizableContainer::enableResize(ResizeDirection direction) {
    resizeFlags |= static_cast<char>(direction);
}

void ResizableContainer::disableResize(ResizeDirection direction) {
    resizeFlags &= ~static_cast<char>(direction);
}

bool ResizableContainer::isResizable(ResizeDirection direction) {
    return resizeFlags & static_cast<char>(direction);
}

ResizableContainer::Ptr ResizableContainer::create(const tgui::Layout2d &size, const tgui::Layout2d &position) {
    return std::make_shared<ResizableContainer>(size, position);
}

ResizableContainer::Ptr ResizableContainer::copy(ResizableContainer::ConstPtr widget) {
    if (widget) {
        return std::static_pointer_cast<ResizableContainer>(widget->clone());
    }
    return nullptr;
}

tgui::Widget::Ptr ResizableContainer::clone() const {
    return std::make_shared<ResizableContainer>(*this);
}

bool ResizableContainer::leftMousePressed(tgui::Vector2f pos) {
    auto absolutePos = pos - getPosition();
    startMousePos = absolutePos;
    startSize = getSize();
    startPosition = getPosition();

    if (isResizable(ResizeDirection::LEFT) && bounded(absolutePos.x, 0, grabberSize)) {
        grabbingFlag = static_cast<char>(ResizeDirection::LEFT);
    } else if (isResizable(ResizeDirection::RIGHT) && bounded(absolutePos.x, getSize().x - grabberSize, getSize().x)) {
        grabbingFlag = static_cast<char>(ResizeDirection::RIGHT);
    } else if (isResizable(ResizeDirection::TOP) && bounded(absolutePos.y, 0, grabberSize)) {
        grabbingFlag = static_cast<char>(ResizeDirection::TOP);
    } else if (isResizable(ResizeDirection::BOTTOM) && bounded(absolutePos.y, getSize().y - grabberSize, getSize().y)) {
        grabbingFlag = static_cast<char>(ResizeDirection::BOTTOM);
    }

    return tgui::Group::leftMousePressed(pos);
}

void ResizableContainer::manualMouseMoved(tgui::Vector2f pos) {
    auto absolutePos = pos - getPosition();

    if (isResizable(ResizeDirection::LEFT) && bounded(absolutePos.x, 0, grabberSize)) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        cursorModified = true;
    } else if (isResizable(ResizeDirection::RIGHT) && bounded(absolutePos.x, getSize().x - grabberSize, getSize().x)) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        cursorModified = true;
    } else if (isResizable(ResizeDirection::TOP) && bounded(absolutePos.y, 0, grabberSize)) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        cursorModified = true;
    } else if (isResizable(ResizeDirection::BOTTOM) && bounded(absolutePos.y, getSize().y - grabberSize, getSize().y)) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        cursorModified = true;
    } else if (cursorModified) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        cursorModified = false;
    }

    tgui::Vector2f deltaMousePos = absolutePos - startMousePos;
    if (grabbingFlag == static_cast<char>(ResizeDirection::LEFT)) {
        float newW = startSize.x.getValue() - deltaMousePos.x;
        newW = std::clamp(newW, static_cast<float>(minResizeWidth), static_cast<float>(maxResizeWidth));
        setSize(newW, startSize.y);
        setPosition(startPosition.x + deltaMousePos.x, startPosition.y);
    } else if (grabbingFlag == static_cast<char>(ResizeDirection::RIGHT)) {
        float newW = startSize.x.getValue() + deltaMousePos.x;
        newW = std::clamp(newW, static_cast<float>(minResizeWidth), static_cast<float>(maxResizeWidth));
        setSize(newW, startSize.y);
    } else if (grabbingFlag == static_cast<char>(ResizeDirection::TOP)) {
        float newH = startSize.y.getValue() - deltaMousePos.y;
        newH = std::clamp(newH, static_cast<float>(minResizeHeight), static_cast<float>(maxResizeHeight));
        setSize(startSize.x, newH);
        setPosition(startPosition.x, startPosition.y + deltaMousePos.y);
    } else if (grabbingFlag == static_cast<char>(ResizeDirection::BOTTOM)) {
        float newH = startSize.y.getValue() + deltaMousePos.y;
        newH = std::clamp(newH, static_cast<float>(minResizeHeight), static_cast<float>(maxResizeHeight));
        setSize(startSize.x, newH);
    }
    onResize.emit(this, tgui::Layout2d{getSize().x - startSize.x, getSize().y - startSize.y});
}
void ResizableContainer::mouseMoved(tgui::Vector2f pos) {
    tgui::Group::mouseMoved(pos);
    if (!useExternalMouseEvent) {
        manualMouseMoved(pos);
    }
}

void ResizableContainer::manualLeftMouseReleased(tgui::Vector2f pos) {
    grabbingFlag = 0;
}

void ResizableContainer::leftMouseReleased(tgui::Vector2f pos) {
    tgui::Group::leftMouseReleased(pos);
    if (!useExternalMouseEvent) {
        manualLeftMouseReleased(pos);
    }
}
