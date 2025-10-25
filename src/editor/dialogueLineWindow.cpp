#include "windows/dialogueLineWindow.hpp"
#include "dialogueBalloon.hpp"
#include "editor.hpp"
#include "projectFile.hpp"
#include "windowPopup.hpp"
#include <cstring>
#include <raylib.h>
#include <imgui.h>
#include "editor.hpp"
#include "fileSystemService.hpp"

DialogueLineWindow::DialogueLineWindow() {}

DialogueLineWindow::DialogueLineWindow(Rectangle rect)
: WindowPopup("New Dialogue Line..", rect)
{
    this->rect = rect;
    strcpy(lineTitle, "");
}

void DialogueLineWindow::openWindow()
{
    WindowPopup::openWindow();
}

void DialogueLineWindow::closeWindow()
{
    WindowPopup::closeWindow();
}

void DialogueLineWindow::draw()
{
    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("New Dialogue Line..")) {
        ImGui::InputText("Title", lineTitle, IM_ARRAYSIZE(lineTitle));

        if (ImGui::Button("Submit")) {
            if (Editor::getFileSystem().isAvailable(FILE_DIALOGUE)) {
                FileSystemService& fs = Editor::getFileSystem();
                Dialogue* diag = fs.getCurrentFile()->getData<Dialogue>();
                diag->lines.push_back(DialogueLine {
                    "Character", "Content"
                });
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            closeWindow();
        }
        ImGui::EndPopup();
    }
}
