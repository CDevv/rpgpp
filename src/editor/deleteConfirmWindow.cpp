#include "deleteConfirmWindow.hpp"
#include <cstdio>
#include <raylib.h>
#include <imgui.h>
#include <filesystem>
#include <system_error>
#include "editor.hpp"
#include "fileSystemService.hpp"

DeleteConfirmWindow::DeleteConfirmWindow() {}

DeleteConfirmWindow::DeleteConfirmWindow(Rectangle rect)
{
    this->active = false;
    this->rect = rect;
    this->filePath = "";
}

void DeleteConfirmWindow::setActive()
{
    this->active = true;
    ImGui::OpenPopup("Confirm Delete..");
}

void DeleteConfirmWindow::closeWindow()
{
    this->active = false;
    this->filePath = "";
    ImGui::CloseCurrentPopup();
}

void DeleteConfirmWindow::setFilePath(std::string filePath)
{
    this->filePath = filePath;
}

void DeleteConfirmWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("Confirm Delete..")) {
        std::string textContent = TextFormat("Are you sure you want to delete %s?",
            GetFileName(filePath.c_str()));
        ImGui::Text("%s", textContent.c_str());

        if (ImGui::Button("Yes")) {
            std::error_code ec;
            std::filesystem::remove(filePath, ec);
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
