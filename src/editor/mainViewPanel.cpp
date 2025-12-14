#include <mainViewPanel.hpp>
#include <IconsKenney.h>
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "imgui.h"
#include "rlImGui.h"

MainViewPanel::MainViewPanel() {}

MainViewPanel::MainViewPanel(Rectangle rect)
{
	this->rect = rect;
	this->logoTexture = LoadTexture("resources/logo.png");
}

void MainViewPanel::setRect(Rectangle rect)
{
	this->rect = rect;
}

void MainViewPanel::update()
{

}

void MainViewPanel::draw()
{
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	float indent = ((rect.width / 2) - (static_cast<float>(logoTexture.width) / 2));

	ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
	ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
	if (ImGui::Begin("Main View Panel", nullptr,
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse)) {
		ImGui::Indent(indent);
		rlImGuiImage(&logoTexture);

		ImGui::Indent(-indent);
		if (ImGui::Button("Create Project..", ImVec2(-1, 24))) {
			ui.getWindowContainer().open("ProjectInit");
		}
		if (ImGui::Button("Open Project..", ImVec2(-1, 24))) {
			fs.promptOpenProject();
			ui.setInitial();
		}

		ui.getWindowContainer().drawWindow("ProjectInit");

		ImGui::End();
	}
}
