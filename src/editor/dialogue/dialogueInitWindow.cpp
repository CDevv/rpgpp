#include "windows/dialogueInitWindow.hpp"

#include <imgui.h>
#include <raylib.h>

#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "windowPopup.hpp"

DialogueInitWindow::DialogueInitWindow() : rect(), title{}
{
}

DialogueInitWindow::DialogueInitWindow(Rectangle rect)
: WindowPopup("New Dialogue..", rect)
{
    strcpy(title, "");
    this->rect = rect;
}

void DialogueInitWindow::closeWindow()
{
    EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
    ui.setMouseLock(false);

    strcpy(title, "");
    WindowPopup::closeWindow();
}

void DialogueInitWindow::openWindow()
{
    WindowPopup::openWindow();
}

void DialogueInitWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("New Dialogue..")) {
        ImGui::InputText("Title", title, 256);

        if (ImGui::Button("Submit")) {
            titleText = title;

            if (!titleText.empty()) {
                std::string filePath = TextFormat("dialogues/%s.rdiag", titleText.c_str());

                std::string diagFileContent = R"(
                {
                "diag": [["Character","Content"]]
                }
                )";
                SaveFileText(TextFormat("dialogues/%s.rdiag", titleText.c_str()), diagFileContent.data());

                fs.getProject()->reloadPaths();
                fs.openProjectFile(filePath);
                ui.setInitial();

                closeWindow();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            closeWindow();
        }

        ImGui::EndPopup();
    }
}
