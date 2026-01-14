#include "windows/projectInitWindow.hpp"

#include <imgui.h>

#include "editor.hpp"
#include "editorInterfaceService.hpp"
#include "fileSystemService.hpp"
#include "project.hpp"
#include "windowPopup.hpp"

ProjectInitWindow::ProjectInitWindow() : rect(), titleEditMode(false), title{}, hasSetDirPath(false) {
}

ProjectInitWindow::ProjectInitWindow(Rectangle rect)
: WindowPopup("New Project..", rect)
{
	this->rect = rect;

	this->titleEditMode = false;
    this->titleText = "";
    this->hasSetDirPath = false;
    this->dirPath = "";

    strcpy(this->title, "");
}

void ProjectInitWindow::openWindow()
{
    WindowPopup::openWindow();
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

    WindowPopup::closeWindow();
}

void ProjectInitWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

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
