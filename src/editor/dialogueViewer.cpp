#include "dialogueViewer.hpp"
#include "dialogueBalloon.hpp"
#include <imgui.h>
#include <raylib.h>
#include <Str.h>
#include <imgui_stdlib.h>

DialogueViewer::DialogueViewer() {}

DialogueViewer::DialogueViewer(Rectangle rect)
{
    this->rect = rect;
    //str = "Test.";
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

    ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
    ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
    if (ImGui::Begin("DialogueView", NULL,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("%s | Lines count: %i", diag->title.c_str(), diag->lines.size());
        ImGui::SameLine();
        ImGui::Button("New Line..");

        int idx = 0;
        for (DialogueLine line : diag->lines) {
            if (ImGui::BeginChild(TextFormat("line-%i", idx), ImVec2(-1, 94),
                ImGuiChildFlags_Borders)) {
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
