#include "windows/deleteConfirmWindow.hpp"
#include <cstdio>
#include <raylib.h>
#include <imgui.h>
#include <filesystem>
#include <system_error>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "windowPopup.hpp"

DeleteConfirmWindow::DeleteConfirmWindow() {}

DeleteConfirmWindow::DeleteConfirmWindow(Rectangle rect)
: WindowPopup("Confirm Delete..", rect)
{
    this->rect = rect;
}

void DeleteConfirmWindow::openWindow()
{
    WindowPopup::openWindow();
}

void DeleteConfirmWindow::closeWindow()
{
    WindowPopup::closeWindow();
}

void DeleteConfirmWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("Confirm Delete..")) {
        std::string textContent = TextFormat("Are you sure you want to delete %s?",
            GetFileName(getProp().c_str()));
        ImGui::Text("%s", textContent.c_str());

        if (ImGui::Button("Yes")) {
            std::error_code ec;
            std::filesystem::remove(getProp(), ec);
            if (ec.value() != 0) {
                printf("filesystem error: %s", ec.message().c_str());
            }

            fs.getProject()->reloadPaths();
            closeWindow();
        }
        ImGui::SameLine();

        if (ImGui::Button("No")) {
            closeWindow();
        }

        ImGui::EndPopup();
    }
}
