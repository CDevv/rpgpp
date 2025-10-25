#include "dialogueViewer.hpp"
#include "IconsKenney.h"
#include "dialogueBalloon.hpp"
#include <cstring>
#include <imgui.h>
#include <raylib.h>
#include <imgui_stdlib.h>

DialogueViewer::DialogueViewer() {}

DialogueViewer::DialogueViewer(Rectangle rect)
{
    this->rect = rect;
    strcpy(newLineName, "");
}

void DialogueViewer::setRect(Rectangle rect)
{
    this->rect = rect;
}

void DialogueViewer::setDialogue(Dialogue* diag)
{
    this->diag = diag;
}

void DialogueViewer::update()
{

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
                "Character", "Content"
            });
        }

        int idx = 0;
        for (std::vector<DialogueLine>::iterator i = diag->lines.begin(); i != diag->lines.end(); ++i) {
            if (ImGui::BeginChild(TextFormat("line-%i", idx), ImVec2(-1, 120),
                ImGuiChildFlags_Borders)) {
                if (ImGui::Button(ICON_KI_TRASH_ALT " Delete Line")) {
                    diag->lines.erase(i);
                    ImGui::EndChild();
                    break;
                }

                ImGui::InputText("Character", &diag->lines[idx].characterName);
                ImGui::InputTextMultiline("##Content", &diag->lines[idx].text,
                    ImVec2(-1, 48));
                ImGui::EndChild();
            }

            idx++;
        }

        ImGui::End();
    }
}
