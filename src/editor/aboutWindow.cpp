#include "windows/aboutWindow.hpp"
#include <imgui.h>
#include <raylib.h>
#include "game.hpp"
#include <rlImGui.h>
#include "editor.hpp"
#include "windowPopup.hpp"

AboutWindow::AboutWindow() {}

AboutWindow::AboutWindow(Rectangle rect)
: WindowPopup("About RPG++", rect)
{
    this->rect = rect;
}

void AboutWindow::openWindow()
{
    WindowPopup::openWindow();
}

void AboutWindow::closeWindow()
{
    WindowPopup::closeWindow();
}

void AboutWindow::draw()
{
    Texture2D logo = Editor::getResources().getTexture("logo");

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("About RPG++")) {
        rlImGuiImage(&logo);

        ImGui::TextLinkOpenURL("RPG++ Website", "https://rpgpp.dev");

        ImGui::Separator();

        ImGui::Text("RPG++ v%s", RPGPP_VER);
        ImGui::Text("raylib v%s", RAYLIB_VERSION);
        ImGui::Text("Dear ImGui v%s", IMGUI_VERSION);
        if (ImGui::Button("Close")) {
            closeWindow();
        }
        ImGui::EndPopup();
    }
}
