#include "windowPopup.hpp"
#include <imgui.h>

WindowPopup::WindowPopup() {}

WindowPopup::WindowPopup(std::string imPopupId, Rectangle rect)
{
    this->imPopupId = imPopupId;
    this->rect = rect;
    this->propExists = false;
    this->prop = "";
}

void WindowPopup::openWindow()
{
    ImGui::OpenPopup(imPopupId.c_str());
}

void WindowPopup::closeWindow()
{
    ImGui::CloseCurrentPopup();
}

void WindowPopup::update() {}

void WindowPopup::setProp(std::string prop)
{
    this->propExists = true;
    this->prop = prop;
}

std::string WindowPopup::getProp()
{
    return prop;
}

bool WindowPopup::hasProp()
{
    return propExists;
}
