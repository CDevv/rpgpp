#include "windowPopup.hpp"

#include <imgui.h>

#include "editor.hpp"

WindowPopup::WindowPopup() : rect(), propExists(false)
{
}

WindowPopup::WindowPopup(const std::string &imPopupId, Rectangle rect)
{
    this->imPopupId = imPopupId;
    this->rect = rect;
    this->propExists = false;
    this->prop = "";
}

void WindowPopup::openWindow()
{
    //ImGui::OpenPopup(imPopupId.c_str());
    Editor::getUi().getGui()->add(childWindow);
}

void WindowPopup::closeWindow()
{
    //ImGui::CloseCurrentPopup();
    Editor::getUi().getGui()->remove(childWindow);
}

void WindowPopup::update() {}

void WindowPopup::setProp(const std::string &prop)
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
