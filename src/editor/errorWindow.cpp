#include "windows/errorWindow.hpp"

#include <imgui.h>
#include <raylib.h>

#include "windowPopup.hpp"

ErrorWindow::ErrorWindow() : rect()
{
}

ErrorWindow::ErrorWindow(Rectangle rect)
: WindowPopup("Runtime Error", rect)
{
    this->rect = rect;
}

void ErrorWindow::openWindow()
{
    WindowPopup::openWindow();
}

void ErrorWindow::closeWindow()
{
    WindowPopup::closeWindow();
}

void ErrorWindow::draw()
{
    ImGui::SetNextWindowSize(ImVec2 { 240, -1 });
    if (ImGui::BeginPopupModal("Runtime Error")) {
        ImGui::TextWrapped("%s", getProp().c_str());
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
