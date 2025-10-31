#include "dialogueViewer.hpp"
#include "IconsKenney.h"
#include "dialogueBalloon.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "nfd.h"
#include "rlImGui.h"
#include <algorithm>
#include <cstring>
#include <imgui.h>
#include <raylib.h>
#include <imgui_stdlib.h>

DialogueViewer::DialogueViewer() {}

DialogueViewer::DialogueViewer(Rectangle rect)
{
    this->rect = rect;
    strcpy(newLineName, "");

    this->images = std::map<std::string, Texture2D>{};
}

void DialogueViewer::setRect(Rectangle rect)
{
    this->rect = rect;
}

void DialogueViewer::setDialogue(Dialogue* diag)
{
    this->diag = diag;

    for (DialogueLine line : diag->lines) {
        if (line.hasPortrait) {
            Texture2D text = LoadTexture(TextFormat("%s/%s/%s",
                Editor::getFileSystem().getProject()->getProjectBasePath().c_str(),
                "images", line.imageId.c_str()));
            images[line.imageId] = text;
        }
    }
}

void DialogueViewer::update()
{

}

bool DialogueViewer::drawDialogueLine(int idx, std::vector<DialogueLine>::iterator it)
{
    if (ImGui::BeginChild(TextFormat("line-%i", idx), ImVec2(-1, 120),
        ImGuiChildFlags_Borders)) {

        if (ImGui::Button(ICON_KI_ARROW_TOP " Move Up")) {
            if (idx != 0) {
                std::iter_swap(it, it - 1);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_KI_ARROW_BOTTOM " Move Down")) {
            if (it != diag->lines.cend()) {
                std::iter_swap(it, it + 1);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_KI_TRASH_ALT " Delete Line")) {
            diag->lines.erase(it);
            ImGui::EndChild();
            return true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Has Portrait", &diag->lines[idx].hasPortrait);

        if (diag->lines[idx].hasPortrait) {
            ImGui::BeginGroup();
            if (rlImGuiImageButtonSize(TextFormat("line-%i", idx), &images[diag->lines[idx].imageId.c_str()],
                Vector2 { 56, 56 })) {
                FS_Result fsResult = Editor::getFileSystem().openImage();
                if (fsResult.result == NFD_OKAY) {
                    diag->lines[idx].imageId = GetFileName(fsResult.path.c_str());

                    Texture2D text = LoadTexture(TextFormat("%s/%s/%s",
                        Editor::getFileSystem().getProject()->getProjectBasePath().c_str(),
                        "images", diag->lines[idx].imageId.c_str()));
                    images[diag->lines[idx].imageId] = text;
                }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
        }

        ImGui::BeginGroup();
        ImGui::InputText("Character", &diag->lines[idx].characterName);
        ImGui::InputTextMultiline("##Content", &diag->lines[idx].text,
            ImVec2(-1, 48));
        ImGui::EndGroup();
        ImGui::EndChild();
    }

    return false;
}

void DialogueViewer::draw()
{
    if (diag == nullptr) return;

    bool newLineOpen = false;

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("DialogueView", NULL,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("%s | Lines count: %i", diag->title.c_str(), diag->lines.size());
        ImGui::SameLine();
        if (ImGui::Button("New Line..")) {
            diag->lines.push_back(DialogueLine {
                "Character", "Content", false, ""
            });
        }

        int idx = 0;
        for (std::vector<DialogueLine>::iterator i = diag->lines.begin(); i != diag->lines.end(); ++i) {
            if (drawDialogueLine(idx, i)) {
                break;
            }

            idx++;
        }

        ImGui::End();
    }
}
