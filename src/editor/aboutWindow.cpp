#include "windows/aboutWindow.hpp"
#include <imgui.h>
#include <raylib.h>
#include "game.hpp"
#include <rlImGui.h>
#include "editor.hpp"

AboutWindow::AboutWindow() {}

AboutWindow::AboutWindow(Rectangle rect)
{
    this->active = false;
    this->rect = rect;
}

void AboutWindow::setActive()
{
    this->active = true;
    ImGui::OpenPopup("About RPG++");
}

void AboutWindow::closeWindow()
{
    this->active = false;
    ImGui::CloseCurrentPopup();
}

void AboutWindow::draw()
{
    Texture2D logo = Editor::getUi().getLogoTexture();

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
