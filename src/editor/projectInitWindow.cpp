#include "projectInitWindow.hpp"
#include <imgui.h>
#include "project.hpp"
#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"

ProjectInitWindow::ProjectInitWindow() {}

ProjectInitWindow::ProjectInitWindow(Rectangle rect)
{
	this->rect = rect;
	this->active = false;

	this->titleEditMode = false;
    this->titleText = "";
    this->hasSetDirPath = false;
    this->dirPath = "";

    strcpy(this->title, "");
}

void ProjectInitWindow::setActive()
{
	active = true;
	ImGui::OpenPopup("New Project..");
}

void ProjectInitWindow::closeWindow()
{
	EditorInterfaceService& ui = Editor::getUi();
    ui.setMouseBoxLayer(VIEWBOX_LAYER_BASE);
    ui.setMouseLock(false);

    dirPath = "";
    hasSetDirPath = false;
    titleText = "";
    strcpy(this->title, "");
    active = false;

    ImGui::CloseCurrentPopup();
}

void ProjectInitWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

	ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
	if (ImGui::BeginPopupModal("New Project..")) {
	    ImGui::InputText("Title", title, IM_ARRAYSIZE(title));

		dirPath.push_back('\0');
		ImGui::InputText("Location", dirPath.data(), dirPath.size(), ImGuiInputTextFlags_ReadOnly);

		if (ImGui::Button("Browse..", ImVec2(-1, 0))) {
			FS_Result fsResult = fs.openFolder();
            if (fsResult.result == NFD_OKAY) {
                dirPath = fsResult.path;
                hasSetDirPath = true;
            }
		}

		if (ImGui::Button("Submit..")) {
			titleText = title;

			bool validated = !titleText.empty() && hasSetDirPath;

            if (validated) {
                Project::generateNewProj(titleText, dirPath);

                fs.setProject(TextFormat("%s/%s/proj.rpgpp", dirPath.c_str(), titleText.c_str()));

                closeWindow();
            }
        }
		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			closeWindow();
		}

	    ImGui::EndPopup();
	}
}
